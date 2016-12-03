/*
Things to Do:
	Create kobject Eudyptula
	Create callback functions for ID, Jiffies and Foo.
	Create Atrributes ID, jiffies, and Foo.
	Create sysfs files for each attribute
			
*/
/*
Explanation:	
	For every file is the Sysfs directory we need to create a struct 
	"kobj_attribute" for it.
	There are two functions and two variables assocaited with every 
	kobj_attribute.
		1- Name
		2- Mode
		3- Show
		4- Store
	
	kobj_attribute is defined in file /linux/kobject.h:139
	struct kobj_attribute {
	struct attribute attr;
	ssize_t (*show)(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf);
	ssize_t (*store)(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count);
	};

	"Attribute" is defined in file /linux/sysfs.h:29
	struct attribute {
	const char		*name;
	umode_t			mode;
	};
	
	Use sysfs_create_files to create files.
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <linux/jiffies.h>
#include <linux/spinlock.h>
#include <linux/kobject.h>
#include <linux/jiffies.h>

#define DRIVER_AUTHOR "Mohsin Shan"
#define DRIVER_DESC "A simple driver that prints Hello World"
#define DIRECTORY "Eudyptula"


#define ID_LEN 13
static const char ID[] = "3330c197cbcb";
static char foo[4096];

ssize_t id_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	return sprintf(buf, "%s\n", ID);
}

ssize_t id_write(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t len)
{
	char recv[len];	
	if (len > ID_LEN)
		return -EINVAL;
	
	sprintf(recv, "%s", buf);
	recv[ID_LEN - 1] = '\0';
	
	if (strcmp(recv, ID))
		return -EINVAL;
	return len;
	
}
struct kobj_attribute id_attribute = __ATTR(ID, 0664, id_show, id_write);

ssize_t jiffies_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	return sprintf(buf, "%ld\n", jiffies);
}
struct kobj_attribute jiffies_attribute = __ATTR(jiffies, 0444, jiffies_show, NULL);

ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	return sprintf(buf, "%s\n", foo);
}

ssize_t foo_write(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t len)
{
	if (len > 4096)
		return -EINVAL;
	
	return sprintf(foo, "%s", buf);	
}
struct kobj_attribute foo_attribute = __ATTR(foo, 0644, foo_show, foo_write);

struct kobject	*kobj;

const struct attribute *attr[] = {
	&id_attribute.attr,
	&foo_attribute.attr,
	&jiffies_attribute.attr
};

int init_module(void)
{
	kobj = kobject_create_and_add("Eudypa", NULL);
	if(!kobj)
		return -ENOMEM;
		
	sysfs_create_files(kobj, attr);	
		
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
