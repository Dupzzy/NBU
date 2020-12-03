// SPDX-License-Identifier: GPL
// ## References used ##
// https://embetronicx.com/tutorials/linux/device-drivers/linux-device-driver-part-1-introduction/
// on the left side there is a menu with navigation and topics covered
// https://tldp.org/LDP/lkmpg/2.4/html/x579.html
// https://www.youtube.com/watch?v=Zi6ooCultI0
// The first 3 links are great source for understanding how to and the ways to allocate
// major number, adding the device to the system, creating device file and other
// https://www.opensourceforu.com/2017/01/regmap-reducing-redundancy-linux-code/
// Probably the best source for regmap for newbies

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/regmap.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/slab.h>

// Buffer size 9 is because I'm expecting 3 bytes for the integer part of the number, 1 byte
// for the floating point, 4 bytes for the fraction part and 1 byte for terminating 0
// Using 9 and not 10 because I'm not putting the sign in the buffer, instead directly in userspace
#define		BUFF_SIZE		9
#define		TMP100_REG_00		0x00
#define		TMP100_REG_MAX		0x01
#define		DRIVER_NAME		"tmp100"

static ssize_t tmp100_read(struct file *, char *, size_t, loff_t *);
static int TMP100_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int TMP100_i2c_remove(struct i2c_client *client);
static int tmp100_open(struct inode *inode, struct file *file);
static int tmp100_release(struct inode *inode, struct file *file);

struct tmp100 {
	// dev_name is gonna store the name of the specific device
	// The size of dev_name is 11 as it follows => TMP100_<minor number of corresponding instance>
	// which is 10 bytes at max + 1 for the '\0'
	char dev_name[30];
	dev_t dev;			// Used to store the major and minor number
	struct class *dev_class;	// Class that this device should be registered to
	struct cdev tmp100_cdev;	// Kernel’s internal structure that represents char devices
	struct regmap *regmap;		// Structure initialized with client (device specific data) and regmap_config
	struct mutex mutex;		// Mutex used to controll hardware access
};

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = tmp100_read,
	.open = tmp100_open,
	.release = tmp100_release,
};

static const struct regmap_config tmp100_regmap_config = {
	.reg_bits = 8,
	.val_bits = 16,
	.max_register = TMP100_REG_MAX,
};

static int tmp100_open(struct inode *inode, struct file *file)
{
	struct tmp100 *tmp100;

	// Getting the adress of the structure we are creating in the probe func
	tmp100 = container_of(inode->i_cdev, struct tmp100, tmp100_cdev);
	if (tmp100 == NULL) {
		pr_debug("Container_of error...\n");
		return -ENODEV;
	}

	file->private_data = tmp100;
	pr_debug("Device File Opened...!!!\n");

	return 0;
}

static int tmp100_release(struct inode *inode, struct file *file)
{
	pr_debug("Device File Closed...!!!\n");
	return 0;
}

static ssize_t tmp100_read(struct file *file, char __user *buffer_user, size_t length, loff_t *offset)
{
	struct tmp100 *tmp100 = file->private_data;
	// Variable used to store the error code of regmap_read if it fails.
	int error;
	// Variable used to store the wread value from the regmap_read func
	uint32_t ur_val;
	// Variable used to store the 16 bits read in ur_val to r_val
	int16_t r_val;
	// Buffer storing the info to be outputted in user space
	int8_t output[BUFF_SIZE];
	// Since I am multiplying by 10000, to get the fraction number to have 4 digits
	//I need to store it in "enough big" integer
	uint16_t fraction = 0;
	// The integer part is just 8 bits and it doesn't change to be any bigger
	uint8_t integer_part = 0;

	// To mark the end of reading we compare offset to 0, which means if bigger than 0 we are done reading
	if (*offset != 0)
		return 0;

	mutex_lock(&tmp100->mutex);
	error = regmap_read(tmp100->regmap, TMP100_REG_00, &ur_val);
	mutex_unlock(&tmp100->mutex);

	if (error < 0)
		return error;

	r_val = ur_val;

	if (r_val < 0 && length) {
		if (put_user('-', buffer_user++) != 0)
			return -EFAULT;
		(*offset)++;
		r_val = -r_val;
		length--;
	}

	// For example if we get -10 in r_val, r_val is 16 bit
	// Before r_val = -r_rval; 1111 0110 0000 0000 ~ 0xfffff600
	// r_val = -r_val;
	// r_val = ~r_val + 1;
	// 0000 1001 1111 1111 + 1
	// After: 0000 1010 0000 0000 positive 10d ~ 0x00000a00

	// The number stored in r_val has 12 effective bits which are stored in the 12 MSB bits in r_val
	// From these 12 bites, the 8 MSB of them are used for integer part of the temperature
	integer_part =  (uint16_t)r_val >> 8;

	// And the 4 LSB of these 12 bits are used for the fraction part,
	// which means we can have 16 different fraction parts (0 - 15)
	fraction = (r_val >> 4) & 0x0f; // This way we are getting the 4 bits needed for the fraction part

	// So depending on where we want our floating point to be and how much numbers to have
	// after it we multiply the number by 10 (for 1 number), 100 (for 2 numbers) etc..
	// multiplying by 10000 means 4 numbers after the foating point
	// and " >> 4" is the same as dividing by 16 which will result in our fraction part
	fraction = (fraction * 10000) >> 4;

	snprintf(output, BUFF_SIZE, "%d.%04d", integer_part, fraction);

	while (output[*offset] && length) {
		if (put_user(output[(*offset)++], buffer_user++) != 0) // put_user returns 0 on success and -EFAULT on erorr
			return -EFAULT;
		length--;
	}

	// Partial reads are not supported
	if (length) {
		if (put_user('\n', buffer_user++) != 0)
			return -EFAULT;
		(*offset)++;
		length--;
	} else
		return -ENOBUFS; // No buffer space available

	return *offset;
}

/* Module init function */
static int TMP100_i2c_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	struct tmp100 *tmp100;

	// Allocating memory for tmp100
	tmp100 = kzalloc(sizeof(struct tmp100), GFP_KERNEL);

	tmp100->dev = 0;
	tmp100->regmap = devm_regmap_init_i2c(client, &tmp100_regmap_config);

	mutex_init(&tmp100->mutex);

	if (IS_ERR(tmp100->regmap))
		return PTR_ERR(tmp100->regmap);

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		pr_err("I2C_FUNC_I2C not supported\n");
		return -ENODEV;
	}

	/*Allocating Major number*/
	if ((alloc_chrdev_region(&tmp100->dev, 0, 1, DRIVER_NAME)) < 0) {
		pr_debug("Cannot allocate major number\n");
		return -1;
	}

	/*Creating cdev structure*/
	cdev_init(&tmp100->tmp100_cdev, &fops);

	/*Adding character device to the system*/
	if ((cdev_add(&tmp100->tmp100_cdev, tmp100->dev, 1)) < 0) {
		pr_debug("Cannot add the device to the system\n");
		goto error_1;
	}

	/*Creating struct class*/
	tmp100->dev_class = class_create(THIS_MODULE, DRIVER_NAME);
	if (tmp100->dev_class == NULL) {
		pr_debug("Cannot create the struct class\n");
		goto error_1;
	}

	// Assigning unique name for this driver instance
	snprintf(tmp100->dev_name, sizeof(tmp100->dev_name),
		"%s_%d", id->name, MINOR(tmp100->dev));
	// I don't understand why id->name writes "\(null\)" and my device file name is : "\(null\)_<minor number>"

	/*Creating device*/
	if ((device_create(tmp100->dev_class, NULL, tmp100->dev, NULL, tmp100->dev_name)) == NULL) {
		pr_debug("Cannot create the Device 1\n");
		goto error_2;
	}
	pr_debug("Device Driver Insert...Done!!!\n");

	i2c_set_clientdata(client, tmp100);
	return 0;

error_2:
	class_destroy(tmp100->dev_class);
error_1:
	if (tmp100 != NULL)
		kfree(tmp100);
	mutex_destroy(&tmp100->mutex);
	unregister_chrdev_region(tmp100->dev, 1);
	return -1;
}

/* Module exit func */
static int TMP100_i2c_remove(struct i2c_client *client)
{
	struct tmp100 *tmp100;
	tmp100 = i2c_get_clientdata(client);

	// Since tmp100 is not global anymore I need to use the i2c_client struct
	// in order to "remove" the module
	device_destroy(tmp100->dev_class, tmp100->dev);
	class_destroy(tmp100->dev_class);
	unregister_chrdev_region(tmp100->dev, 1);
	kfree(tmp100);
	pr_debug("Kernel Module Removed Successfully...\n");

	return 0;
}

static const struct i2c_device_id TMP100_i2c_id[] = {
	{"TMP100", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, TMP100_i2c_id);

static const struct of_device_id TMP100_of_match[] = {
	{ .compatible = "Driver, TMP100"},
	{ },
};

MODULE_DEVICE_TABLE(of, TMP100_of_match);

static struct i2c_driver TMP100_i2c_driver = {
	.driver = {
		.name   = "TMP100_i2c",
		.of_match_table = TMP100_of_match,
		.owner = THIS_MODULE,
	},
	.probe = TMP100_i2c_probe,
	.remove = TMP100_i2c_remove,
	.id_table = TMP100_i2c_id,
};

/* With this function I'm replacing mod init & exit funcs */
module_i2c_driver(TMP100_i2c_driver);

MODULE_DESCRIPTION("TMP 100 driver that can read temperature");
MODULE_AUTHOR("Plamen Damyanov");
MODULE_LICENSE("GPL");
