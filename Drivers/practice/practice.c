#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/kdev_t.h>

dev_t dev = 0;
static struct class *dev_class;

static int __init hello_world_init(void)
{
	printk(KERN_INFO "Welcome to EmbeTronicX\n");
	printk(KERN_INFO "This is the Simple Module\n");
    
	if((alloc_chrdev_region(&dev, 0, 1, "Test")) <0) {
                printk(KERN_INFO "Cannot allocate major number for device 1\n");
                return -1;
	}
	printk(KERN_INFO "Major = %d minor = %d \n", MAJOR(dev), MINOR(dev));
	
	/* creating a struct class */
	if((dev_class = class_create(THIS_MODULE, "Class_ext_module")) == NULL) {
		printk(KERN_INFO "Cannot create the struct clas for device\n");
		unregister_chrdev_region(dev,1);
		return -1;
	}
	
	/* Creating the device */
	if((device_create(dev_class, NULL, dev, NULL, "Class_ext_module")) == NULL) {
		printk(KERN_INFO "Cannot create the device\n");
		class_destroy(dev_class);
		unregister_chrdev_region(dev,1);
		return -1;
	}
	
	printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
	
    return 0;
}
 
/*
** Module Exit function
*/
static void __exit hello_world_exit(void)
{
	unregister_chrdev_region(dev,1);
	class_destroy(dev_class);
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("A simple hello world driver");
MODULE_VERSION("2:1.0");
