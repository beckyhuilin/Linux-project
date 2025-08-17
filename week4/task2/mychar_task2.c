
#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/atomic.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/string.h>

#define DRV_NAME      "mychar"
#define DEF_BUF_SIZE  4096
#define MAX_BUF_SIZE  (16 * 1024 * 1024)

/* IOCTL */
#define MYCHAR_IOC_MAGIC   'M'
#define MYCHAR_IOC_CLEAR       _IO(MYCHAR_IOC_MAGIC, 0)
#define MYCHAR_IOC_SET_BUFSZ   _IOW(MYCHAR_IOC_MAGIC, 1, unsigned long)
#define MYCHAR_IOC_GET_BUFSZ   _IOR(MYCHAR_IOC_MAGIC, 2, unsigned long)
#define MYCHAR_IOC_GET_DLEN    _IOR(MYCHAR_IOC_MAGIC, 3, unsigned long)


static char   *buf;
static size_t  buf_size;
static size_t  data_len;
static atomic_t dev_opened = ATOMIC_INIT(0);  

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
        return 0;
    if (len > data_len - *ppos)
        len = data_len - *ppos;

    return simple_read_from_buffer(ubuf, len, ppos, buf, data_len);
}

static ssize_t dev_write(struct file *f, const char __user *ubuf, size_t len, loff_t *ppos)
{
    ssize_t ret;

    if (*ppos >= buf_size)
        return -ENOSPC;
    if (len > buf_size - *ppos)
        len = buf_size - *ppos;

    ret = simple_write_to_buffer(buf, buf_size, ppos, ubuf, len);
    if (ret > 0 && *ppos > data_len)
        data_len = *ppos;
    return ret;
}



static int resize_buffer(size_t new_sz)
{
    char *newbuf;

    if (new_sz == 0 || new_sz > MAX_BUF_SIZE)
        return -EINVAL;

    newbuf = krealloc(buf, new_sz, GFP_KERNEL);
    if (!newbuf)
        return -ENOMEM;

    
    if (new_sz > buf_size)
        memset(newbuf + buf_size, 0, new_sz - buf_size);

    buf = newbuf;
    if (data_len > new_sz)
        data_len = new_sz;
    buf_size = new_sz;
    return 0;
}

static long dev_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    unsigned long val = 0;

    if (_IOC_TYPE(cmd) != MYCHAR_IOC_MAGIC)
        return -ENOTTY;

    switch (cmd) {
    case MYCHAR_IOC_CLEAR:
        memset(buf, 0, buf_size);
        data_len = 0;
        break;
    case MYCHAR_IOC_SET_BUFSZ:
        if (copy_from_user(&val, (void __user *)arg, sizeof(val)))
            return -EFAULT;
        ret = resize_buffer(val);
        break;
    case MYCHAR_IOC_GET_BUFSZ:
        val = buf_size;
        if (copy_to_user((void __user *)arg, &val, sizeof(val)))
            return -EFAULT;
        break;
    case MYCHAR_IOC_GET_DLEN:
        val = data_len;
        if (copy_to_user((void __user *)arg, &val, sizeof(val)))
            return -EFAULT;
        break;
    default:
        ret = -ENOTTY;
        break;
    }
    return ret;
}

static const struct file_operations fops = {
    .owner          = THIS_MODULE,
    .open           = dev_open,
    .release        = dev_release,
    .read           = dev_read,
    .write          = dev_write,
    .unlocked_ioctl = dev_ioctl,
    .llseek         = noop_llseek,
};

static struct miscdevice mychar_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = DRV_NAME,
    .fops  = &fops,
    .mode  = 0666,
};

static int __init mychar_init(void)
{
    int ret;

    buf = kzalloc(DEF_BUF_SIZE, GFP_KERNEL);
    if (!buf)
        return -ENOMEM;
    buf_size = DEF_BUF_SIZE;
    data_len = 0;

    ret = misc_register(&mychar_misc);
    if (ret) {
        kfree(buf);
        return ret;
    }
    pr_info(DRV_NAME ": task2 (nolock, single-open) loaded, def=%zu\n", buf_size);
    return 0;
}

static void __exit mychar_exit(void)
{
    misc_deregister(&mychar_misc);
    kfree(buf);
    pr_info(DRV_NAME ": task2 unloaded\n");
}

module_init(mychar_init);
module_exit(mychar_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("becky");
MODULE_DESCRIPTION("Week4 Task2: dynamic buffer + ioctl");

