#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/rwlock.h>
#include <linux/proc_fs.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include <linux/kobject.h>

static int major_num = 0;
static rwlock_t locker;
static char string[15] = "Heeeey!\n";

ssize_t test_read(struct file *fd, char __user *buff, size_t size, loff_t *off) {
	size_t rc;

	read_lock(&locker);
	rc = simple_read_from_buffer(buff, size, off, string, 15);
	read_unlock(&locker);

	return rc;
}

ssize_t test_write(struct file *fd, const char __user *buff, size_t size, loff_t *off) {
	size_t rc = 0;

	if (size > 15) {
		return -EINVAL;
	}

	write_lock(&locker);
	rc = simple_write_to_buffer(string, 15, off, buff, size);
	write_unlock(&locker);

	return rc;
}

static struct file_operations fops = {
        .owner = THIS_MODULE,
        .read = test_read,
        .write = test_write
};

int init_module(void) {
	pr_info("My test module loaded.\n");
	rwlock_init(&locker);

	major_num = register_chrdev(major_num, "test", &fops);
	if (major_num < 0) {
		return major_num;
	}
	pr_info("Major = %d\n", major_num);

	return 0;
}

void cleanup_module(void) {
	unregister_chrdev(major_num, "test");
	pr_info("My test module unloaded.\n");
}

MODULE_LICENSE("GPL");
