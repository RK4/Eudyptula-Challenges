/*
Things to Do:
	Create a struct "miscdevice". 
	Associate dynamic minor number to it.
	Create a struct "file_operations"
	Write read and write functions for this misc driver.
	Populate the file_operations struct with these functions.
	Register the device using function in /include/linux/miscdevice.h:71
	int misc_register(struct miscdevice *misc);	
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>

#define DRIVER_AUTHOR "Mohsin Shan"
#define DRIVER_DESC "A simple driver that prints Hello World"
#define ID_LEN 13

static const char ID[ID_LEN] = "3330c197cbcb";
static char recv[13];

static ssize_t read(struct file *file,
		char __user *buf, size_t len, loff_t *ppos)
{
	return simple_read_from_buffer(buf, len, ppos, ID, ID_LEN);
}

static ssize_t write(struct file *file,
		const char __user *buf, size_t len, loff_t *ppos)
{
	size_t ret;

	if (len > ID_LEN)
		return -EINVAL;

	ret = simple_write_to_buffer(recv, ID_LEN, ppos, buf, len);

	if (ret < 0)
		return ret;

	recv[ID_LEN - 1] = '\0';

	if (strcmp(recv, ID))
		return -EINVAL;
	return len;
}

const struct file_operations fops = {
	.read = read,
	.write = write
};

static struct miscdevice my_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "eudyptula",
	.fops = &fops
};

int init_module(void)
{
	int retval;

	retval = misc_register(&my_dev);
	if (retval) {
		pr_debug("Error in registering device");
		return retval;
	}

	pr_debug("Hello World\n");
	return 0;
}

void cleanup_module(void)
{
	misc_deregister(&my_dev);
	pr_debug("Goodbye World\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
