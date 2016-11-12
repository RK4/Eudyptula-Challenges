#include <linux/module.h>
#include <linux/kernel.h>

#define DRIVER_AUTHOR "Mohsin Shan"
#define DRIVER_DESC "A simple driver that prints Hello World"

int init_module(void)
{
	pr_debug("Hello World\n");
	return 0;
}


void cleanup_module(void)
{
	pr_debug("Goodbye World\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
