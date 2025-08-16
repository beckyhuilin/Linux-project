#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/miscdevice.h> 
#include <linux/fs.h> 
#include <linux/uaccess.h> 
#include <linux/atomic.h> 
#include <linux/kernel.h> 
#include <linux/string.h>

#define DRV_NAME "mychar" // 设备名：/dev/mychar 
#define BUF_SIZE 4096 // 固定缓冲区大小（可按需调整） 

static char buf[BUF_SIZE]; 
static size_t data_len;  
static atomic_t dev_opened = ATOMIC_INIT(0); // 0=未占用，1=已占用（单打开者） 


static int dev_open(struct inode *inode, struct file *file) 
{ 
	if (atomic_cmpxchg(&dev_opened, 0, 1) != 0) 
		return -EBUSY;
       	return 0;
} 

static int dev_release(struct inode *inode, struct file *file)
{
       	atomic_set(&dev_opened, 0);
       	return 0;
} 

 
static ssize_t dev_read(struct file *f, char __user *ubuf, size_t len, loff_t *ppos)
{
       	if (*ppos > data_len)
	       	return 0; // EOF
        if (len > data_len - *ppos)
	       	len = data_len - *ppos;
       	return simple_read_from_buffer(ubuf, len, ppos, buf, data_len);
} 

 
static ssize_t dev_write(struct file *f, const char __user *ubuf, size_t len, loff_t *ppos)
{
       	ssize_t ret;
       	if (*ppos >= BUF_SIZE)
	       	return -ENOSPC;
       	if (len > BUF_SIZE - *ppos)
	       	len = BUF_SIZE - *ppos;
       	ret = simple_write_to_buffer(buf, BUF_SIZE, ppos, ubuf, len);
       	if (ret > 0 && *ppos > data_len)
	       	data_len = *ppos; 
        return ret;
} 


static const struct file_operations fops = {
       	.owner = THIS_MODULE,
       	.open = dev_open,
       	.release = dev_release,
       	.read = dev_read,
       	.write = dev_write,
       	.llseek = noop_llseek, 
}; 

static struct miscdevice mychar_misc = {
       	.minor = MISC_DYNAMIC_MINOR,
       	.name = DRV_NAME,
       	.fops = &fops,
       	.mode = 0666, 
}; 

static int __init mychar_init(void)
{
       	data_len = 0;
       	memset(buf, 0, sizeof(buf));
       	pr_info(DRV_NAME ": task1 fixed buffer loaded\n");
       	return misc_register(&mychar_misc);
} 

static void __exit mychar_exit(void)
{
       	misc_deregister(&mychar_misc);
       	pr_info(DRV_NAME ": task1 unloaded\n");
} 

module_init(mychar_init); 
module_exit(mychar_exit); MODULE_LICENSE("GPL"); 

MODULE_AUTHOR("becky"); 
MODULE_DESCRIPTION("Week4 Task1: fixed-size buffer char dev");
