#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/module.h>
#include <linux/hid.h>

#define DRIVER_AUTHOR "Mohsin Shan"
#define DRIVER_DESC "A simple driver that prints Hello World"

static struct usb_device_id device_id[] = {
	{USB_INTERFACE_INFO
	 (USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
	  USB_INTERFACE_PROTOCOL_KEYBOARD)},
	{}
};

MODULE_DEVICE_TABLE(usb, device_id);

int init_module(void)
{
	pr_debug("Hello, USB Keyboard");
	pr_debug("Hello World\n");
	return 0;
}

void cleanup_module(void)
{
	pr_debug("Bye, USB Keyboard");
	pr_debug("Goodbye World\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
