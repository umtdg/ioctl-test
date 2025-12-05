#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/printk.h>

MODULE_AUTHOR("umtdg");
MODULE_DESCRIPTION(
	"Module for testing ioctl on network interfaces created by kernel modules");
MODULE_LICENSE("GPL");

#define IOCTL_TEST_DRIVER_NAME "ioctl-test"

static unsigned int ioctl_test_major = 0;
static unsigned int ioctl_test_minor = 0;

static unsigned int ioctl_test_num_of_dev = 1;
static struct cdev ioctl_test_cdev;

static struct file_operations ioctl_test_dev_fops = {
	.owner = THIS_MODULE,
};

static int __init ioctl_test_mod_init(void)
{
	dev_t dev;
	int ret;

	pr_info("Allocate char device region\n");
	ret = alloc_chrdev_region(&dev, ioctl_test_minor, ioctl_test_num_of_dev,
				  IOCTL_TEST_DRIVER_NAME);
	if (ret) {
		pr_err("Failed to allocate char device region\n");
		return ret;
	}

	ioctl_test_major = MAJOR(dev);

	pr_info("Initialize cdev\n");
	cdev_init(&ioctl_test_cdev, &ioctl_test_dev_fops);
	ret = cdev_add(&ioctl_test_cdev, dev, ioctl_test_num_of_dev);
	if (ret) {
		pr_err("Failed to initialize cdev\n");
		unregister_chrdev_region(dev, ioctl_test_num_of_dev);
		return ret;
	}

	pr_info("Device %d %s installed\n", ioctl_test_major,
		IOCTL_TEST_DRIVER_NAME);

	return 0;
}

static void __exit ioctl_test_mod_exit(void)
{
	dev_t dev = MKDEV(ioctl_test_major, ioctl_test_minor);

	pr_info("Deleting cdev\n");
	cdev_del(&ioctl_test_cdev);

	pr_info("Unregistering char device region\n");
	unregister_chrdev_region(dev, ioctl_test_num_of_dev);

	pr_info("Device %d %s removed\n", ioctl_test_major,
		IOCTL_TEST_DRIVER_NAME);
}

module_init(ioctl_test_mod_init);
module_exit(ioctl_test_mod_exit);
