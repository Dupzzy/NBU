/* ## References used ## */
/* https://embetronicx.com/tutorials/linux/device-drivers/linux-device-driver-part-1-introduction/ -on the left side there is a menu with navigation and topics covered 	*/
/* https://tldp.org/LDP/lkmpg/2.4/html/x579.html 																*/
/* https://www.youtube.com/watch?v=Zi6ooCultI0 																*/
/* The first 3 links are great source for understanding how to and the ways to allocate major number, adding the device to the system, creating device file and other 	*/
/* https://www.opensourceforu.com/2017/01/regmap-reducing-redundancy-linux-code/ - Probably the best source for regmap for newbies 					*/

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

// Buffer size 9 is because I'm expecting 3 bytes for the whole number, 1 byte 
// for the floating point, 4 bytes for the fraction part and 1 byte for terminating 0
// Using 9 and not 10 because I'm not putting the sign in the buffer, instead directly in userspace
#define		BUFF_SIZE		9
#define		TMP100_REG_00		0x00
#define		TMP100_REG_MAX		0x01

static ssize_t tmp100_read(struct file *, char *, size_t, loff_t *);
static int TMP100_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int TMP100_i2c_remove(struct i2c_client *client);
static int tmp100_open(struct inode *inode, struct file *file);
static int tmp100_release(struct inode *inode, struct file *file);

/* Usually I would pack these in a structure */
static struct tmp100 {
	dev_t dev;
	struct class *dev_class;
	struct cdev tmp100_cdev;
	struct regmap *regmap;
	int is_open;
} tmp100;

static struct file_operations fops =
{
    .owner	= THIS_MODULE,
    .read	= tmp100_read,
    .open	= tmp100_open,
    .release	= tmp100_release,
};

static const struct regmap_config tmp100_regmap_config = {
	.reg_bits = 8,
	.val_bits = 16,
	.max_register = TMP100_REG_MAX,
};

static int tmp100_open(struct inode *inode, struct file *file)
{
	if (tmp100.is_open == 1) {
		printk(KERN_INFO "Device is in use...!!!\n");
		return -EBUSY;
	}

    	tmp100.is_open = 1;
        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}

static int tmp100_release(struct inode *inode, struct file *file)
{
	if (tmp100.is_open == 0) {
		printk(KERN_INFO "Device isn't opened..!!!\n");
		return -EPERM;
	}

    	tmp100.is_open = 0;
        printk(KERN_INFO "Device File Closed...!!!\n");
        return 0;
}

static ssize_t tmp100_read(struct file * filp, char *buffer_user, size_t length, loff_t *offset) {
	signed short error; 		/* Variable used to store the error code of regmap_read if it fails.	 							*/
	unsigned int r_val; 		/* Variable used to store the wread value from the regmap_read func 								*/
	char output[BUFF_SIZE];	/* Buffer storing the info to be outputted in user space 									*/
	unsigned short fraction = 0; 	/* Since I am multiplying by 10000, to get the fraction number to have 4 digits I need to store it in "enough big" integer	*/
	unsigned char whole = 0; 	/* The whole part is just 8 bits and it doesn't change to be any bigger 							*/

	if(*offset != 0)
		return 0;

	printk(KERN_INFO "Reading data..\n");

	if ((error = regmap_read(tmp100.regmap, TMP100_REG_00, &r_val)) < 0)
		return error;

	if((signed short)r_val < 0) {
		put_user('-', buffer_user++);
		r_val = -r_val;	
	}

	// The number stored in r_val has 12 effective bits which are stored in the 12 MSB bits in r_val 
	// From these 12 bites, the 8 MSB of them are used for whole part of the temperature 
	whole =  (unsigned short)r_val >> 8;

	// And the 4 LSB of these 12 bits are used for the fraction part, which means we can have 16 different fraction parts (0 - 15)
	fraction = ((unsigned short)r_val >> 4) & 0x0f; // This way we are getting the 4 bits needed for the fraction part 

	// So depending on where we want our floating point to be and how much numbers to have after it we multiply the number by 10 (for 1 number), 100 (for 2 numbers) etc..
	fraction = (fraction * 10000) >> 4; 	// multiplying by 10000 means 4 numbers after the foating point 
						// and " >> 4" is the same as dividing by 16 which will result in our fraction part
			
	snprintf(output, BUFF_SIZE, "%d.%04d", whole, fraction); 
	// In this case muliplying by 10000 means that we are gonna get 4 digits after the the floating point
	// Lets say for example we have 0xf * 10000 = 15000, shifing it 4 timer to the right is like deviding it by 16,
	// which will result in our fraction part of the number

	while(output[*offset])
		put_user(output[(*offset)++], buffer_user++);

	put_user('\n', buffer_user++);
	(*offset)++;

	if(*offset > length)
		return -ENFILE;

	return *offset;
}

/*
** Module init function
*/

static int TMP100_i2c_probe(struct i2c_client *client,
				const struct i2c_device_id *id) {
	tmp100.dev = 0;
	tmp100.regmap = devm_regmap_init_i2c(client, &tmp100_regmap_config);

	if (IS_ERR(tmp100.regmap))
		return PTR_ERR(tmp100.regmap);

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		pr_err("I2C_FUNC_I2C not supported\n");
		return -ENODEV;
	}

         /*Allocating Major number*/
        if((alloc_chrdev_region(&tmp100.dev, 0, 1, "tmp100")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(tmp100.dev), MINOR(tmp100.dev));

        /*Creating cdev structure*/
        cdev_init(&tmp100.tmp100_cdev,&fops);

        /*Adding character device to the system*/
        if((cdev_add(&tmp100.tmp100_cdev,tmp100.dev,1)) < 0) {
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto error_1;
        }

        /*Creating struct class*/
        if((tmp100.dev_class = class_create(THIS_MODULE,"tmp100")) == NULL) {
            printk(KERN_INFO "Cannot create the struct class\n");
            goto error_1;
        }

        /*Creating device*/
        if((device_create(tmp100.dev_class,NULL,tmp100.dev,NULL,"tmp100")) == NULL){
            printk(KERN_INFO "Cannot create the Device 1\n");
            goto error_2;
        }
        printk(KERN_INFO "Device Driver Insert...Done!!!\n");
        return 0;

error_2:
	printk(KERN_INFO "Something is not alright!!\n");
        class_destroy(tmp100.dev_class);
error_1:
	printk(KERN_INFO "Something is not alright!!\n");
        unregister_chrdev_region(tmp100.dev,1);
        return -1;
}

/*
** Module exit function
*/
static int TMP100_i2c_remove(struct i2c_client *client) {
	device_destroy(tmp100.dev_class, tmp100.dev);
        class_destroy(tmp100.dev_class);
        unregister_chrdev_region(tmp100.dev, 1);
        printk(KERN_INFO "Kernel Module Removed Successfully...\n");

        return 0;
}

static const struct i2c_device_id TMP100_i2c_id[] = {
	{"TMP100", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, TMP100_i2c_id);

static const struct of_device_id TMP100_of_match[] = {
	{ .compatible ="Driver, TMP100"},
	{ },
};

MODULE_DEVICE_TABLE(of, TMP100_of_match);

static struct i2c_driver TMP100_i2c_driver = {
	.driver = {
		.name   ="TMP100_i2c",
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
