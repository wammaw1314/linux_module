#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define CHRDEVNAME "chrdev"

static struct cdev chrdev_cdev;
static dev_t chrdev_devno;
static struct class *chrdev_class = NULL;

static int chrdev_open(struct inode *inode, struct file *filp)
{
	printk(KERN_NOTICE "======== chrdev_open.\n");
	return 0;
}

static int chrdev_release(struct inode *inode, struct file *filp)
{
	printk(KERN_NOTICE "======== chrdev_release.\n");
	return 0;
}

static long chrdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	printk(KERN_NOTICE "======== chrdev_ioctl.\n");
	return 0;
}

static const struct file_operations chrdev_fops =
{
	.owner = THIS_MODULE,
	.open = chrdev_open,
	.release = chrdev_release,
	.unlocked_ioctl = chrdev_ioctl,
};

static int chrdev_probe(struct platform_device *dev)
{
	int ret = 0;
	ret = alloc_chrdev_region(&chrdev_devno, 0, 1, CHRDEVNAME);
	if(ret)
	{
		printk(KERN_ALERT "alloc_chrdev_region failed.\n");
		goto PROBE_ERR;
	}
	cdev_init(&chrdev_cdev, &chrdev_fops);
	chrdev_cdev.owner = THIS_MODULE;
	ret = cdev_add(&chrdev_cdev, chrdev_devno, 1);
	if (ret)
	{
		printk(KERN_ALERT "cdev_add failed.\n");
		goto PROBE_ERR_CDEV;
	}
	chrdev_class = class_create(THIS_MODULE, CHRDEVNAME);
	if(IS_ERR(chrdev_class))
	{
		printk(KERN_ALERT "class_create failed.\n");
		goto PROBE_ERR_CLASS;
	}
	if(NULL == device_create(chrdev_class, NULL, chrdev_devno, NULL, CHRDEVNAME))
	{
		printk(KERN_ALERT "device_create failed.\n");
		goto PROBE_ERR_DEV;
	}
	printk(KERN_NOTICE "prob success.\n");
	return 0;

PROBE_ERR_DEV:
	class_destroy(chrdev_class);
PROBE_ERR_CLASS:
	cdev_del(&chrdev_cdev);
PROBE_ERR_CDEV:
	unregister_chrdev_region(chrdev_devno, 1);
PROBE_ERR:
	return -1;
}

static int chrdev_remove(struct platform_device* dev)
{
	printk(KERN_NOTICE "remove device.\n");
	cdev_del(&chrdev_cdev);
	unregister_chrdev_region(chrdev_devno, 1);

	device_destroy(chrdev_class, chrdev_devno);
	class_destroy(chrdev_class);
	return 0;
}

static struct platform_driver chrdev_platform_driver = {
	.probe = chrdev_probe,
	.remove = chrdev_remove,
	.driver = {
		.name = CHRDEVNAME,
		.owner = THIS_MODULE,
	},
};

static void dev_release(struct device * dev)
{
	printk(KERN_NOTICE "dev release.\n");
}

static struct platform_device chrdev_platform_device = {
	.name = CHRDEVNAME,
	.id = 0,
	.dev = {
		.release = dev_release,
	},
};

static int __init chrdev_init(void)
{
	int ret = 0;
	printk(KERN_NOTICE "chrdev init.\n");
	ret = platform_device_register(&chrdev_platform_device);
	if (ret)
	{
		printk(KERN_ALERT "platform_device_register failed.\n");
		return ret;
	}
	ret = platform_driver_register(&chrdev_platform_driver);
	if (ret)
	{
		printk(KERN_ALERT "platform_driver_register failed.\n");
		platform_device_unregister(&chrdev_platform_device);
		return ret;
	}
	return 0;
}

static void __exit chrdev_exit(void)
{
	printk(KERN_NOTICE "chrdev exit.\n");
	platform_device_unregister(&chrdev_platform_device);
	printk(KERN_NOTICE "device unregister.\n");
	platform_driver_unregister(&chrdev_platform_driver);
	printk(KERN_NOTICE "driver unregister.\n");
}

module_init(chrdev_init);
module_exit(chrdev_exit);

MODULE_AUTHOR("YeJoel");
MODULE_LICENSE("GPL");
