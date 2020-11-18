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

#define		BUFF_SIZE		3
#define		TMP100_REG_00		0x00
#define		TMP100_REG_MAX		0x01

static ssize_t tmp100_read(struct file *, char *, size_t, loff_t *);
static int TMP100_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int TMP100_i2c_remove(struct i2c_client *client);
static int tmp100_open(struct inode *inode, struct file *file);
static int tmp100_release(struct inode *inode, struct file *file);

dev_t dev;
struct class *dev_class;
struct cdev tmp100_cdev;
struct regmap *regmap;
int is_open;


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
	if (is_open == 1) {
		printk(KERN_INFO "Device is in use...!!!\n");
		return -1;
	}
    	
    	is_open = 1;
        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}

static int tmp100_release(struct inode *inode, struct file *file)
{
	if (is_open == 0) {
		printk(KERN_INFO "Device isn't opened..!!!\n");
		return -1;
	}
    
    	is_open = 0;
        printk(KERN_INFO "Device File Closed...!!!\n");
        return 0;
}

static ssize_t tmp100_read(struct file * filp, char *buffer, size_t length, loff_t *offset) {
	int bytes_read = 0;
	unsigned int r_val = 0;
	int loop = 0;
	int pos = 0;
	char buff[BUFF_SIZE];
	int error;
	
	if(*offset != 0) 
		return 0;
	
	
	printk(KERN_INFO "Reading data..\n");
		
	error = regmap_read(regmap, TMP100_REG_00, &r_val);
	if (error < 0) {
		printk(KERN_INFO "Error\n");
		return error;
	}
	
	r_val >>= 8;
	/* Check if r_val is negative so we put the sign in the buffer */	
	if ((signed char)r_val < 0) {
		put_user('-', buffer++);
		r_val = ~(signed char)r_val + 1;
	}

	/* Put the data in a char buff that will store it backwards */
	while (length && r_val)  {
		buff[pos++] = (r_val % 10) + '0';
		r_val /= 10;
		length--;
	        bytes_read++;
	}

	loop = bytes_read; 
	
	while (loop) {
	        put_user(buff[loop - 1], buffer++);
		loop--;
	}
	
	if (length < bytes_read) 
		return -EINVAL;
	put_user('\n', buffer++);
	*offset = bytes_read + 1;
	return bytes_read + 1;
}

/*
** Module init function
*/

static int TMP100_i2c_probe(struct i2c_client *client, 
				const struct i2c_device_id *id) {		
	dev = 0;
	regmap = devm_regmap_init_i2c(client, &tmp100_regmap_config);

	if (IS_ERR(regmap))
		return PTR_ERR(regmap);	
		
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		pr_err("I2C_FUNC_I2C not supported\n");
		return -ENODEV;
	}	
				
         /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "tmp100")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        /*Creating cdev structure*/
        cdev_init(&tmp100_cdev,&fops);
 
        /*Adding character device to the system*/
        if((cdev_add(&tmp100_cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }
 
        /*Creating struct class*/
        if((dev_class = class_create(THIS_MODULE,"tmp100")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
        }
 
        /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"tmp100")) == NULL){
            printk(KERN_INFO "Cannot create the Device 1\n");
            goto r_device;
        }
        printk(KERN_INFO "Device Driver Insert...Done!!!\n");
        return 0;
 
r_device:
	printk(KERN_INFO "Something is not alright!!\n");
        class_destroy(dev_class);
r_class:
	printk(KERN_INFO "Something is not alright!!\n");
        unregister_chrdev_region(dev,1);
        return -1;
}
 
/*
** Module exit function
*/
static int TMP100_i2c_remove(struct i2c_client *client) {
	device_destroy(dev_class, dev);
        class_destroy(dev_class);
        unregister_chrdev_region(dev, 1);
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
