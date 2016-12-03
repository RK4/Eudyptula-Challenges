/*
Things to Do:
	Create Eudyptula directory in debugfs filesystem.
	Create Variables ID, foo. Jiffies is already defined in /linux/jiffies.h
	Write callbacks for ID and foo. There is no need for jiffies.
	Create struct file_operations for ID and foo and fill this.
	Create files in debugfs.
*/

/* 
Explanation:
Debugsfs is relatively simple as compared to sysfs.

- Create Directory in debugfs filesystem using function in /linux/debugfs.h:79
struct dentry *debugfs_create_dir(const char *name, struct dentry *parent);

- Create files in the directory using functions below. We are basically 
  exporting only one of the variable from our module to userspace.
struct dentry *debugfs_create_u8(const char *name, umode_t mode,
				 struct dentry *parent, u8 *value);
struct dentry *debugfs_create_u16(const char *name, umode_t mode,
				  struct dentry *parent, u16 *value);
struct dentry *debugfs_create_u32(const char *name, umode_t mode,
				  struct dentry *parent, u32 *value);
struct dentry *debugfs_create_u64(const char *name, umode_t mode,
				  struct dentry *parent, u64 *value);

- It is also possible to export a chunk of data. In this case use the function
struct dentry *debugfs_create_file(const char *name, umode_t mode,
				   struct dentry *parent, void *data,
				   const struct file_operations *fops);
				   
  In this case we"ll have to define file operations functions, and have to
  assocaite them with this file.

*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/dcache.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <linux/jiffies.h>
#include <linux/spinlock.h>

#define DRIVER_AUTHOR "Mohsin Shan"
#define DRIVER_DESC "A simple driver that prints Hello World"
#define DIRECTORY "eudyptula"
#define PARENT_DIRECTORY "/sys/kernel/debug"
#define ID_LEN 13

static struct dentry *debugfs_dir;
static struct dentry *id_dir;


static const char ID[] = "3330c197cbcb";


static ssize_t read_id(struct file *file,  char __user *buf,
			size_t len, loff_t *ppos)
{
	return simple_read_from_buffer(buf, len, ppos, ID, ID_LEN);
}


static ssize_t write_id(struct file *file, const char __user *buf,
			size_t len, loff_t *ppos)
{
	char recv[len];
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

static const struct file_operations fops_id = {
	.read = read_id,
	.write = write_id,
};

int init_module(void)
{
	debugfs_dir = debugfs_create_dir(DIRECTORY, NULL);
	if (debugfs_dir == NULL)
		pr_debug("ERROR");

	id_dir = debugfs_create_file("id", 0666, debugfs_dir,
					 NULL, &fops_id);

	if (id_dir == NULL)
		pr_debug("ERROR");

	pr_debug("Hello World\n");
	return 0;
}


void cleanup_module(void)
{
	debugfs_remove_recursive(debugfs_dir);
	pr_debug("Goodbye World\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
