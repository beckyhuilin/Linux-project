
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define MYCHAR_IOC_MAGIC   'M'
#define MYCHAR_IOC_CLEAR       _IO(MYCHAR_IOC_MAGIC, 0)
#define MYCHAR_IOC_SET_BUFSZ   _IOW(MYCHAR_IOC_MAGIC, 1, unsigned long)
#define MYCHAR_IOC_GET_BUFSZ   _IOR(MYCHAR_IOC_MAGIC, 2, unsigned long)
#define MYCHAR_IOC_GET_DLEN    _IOR(MYCHAR_IOC_MAGIC, 3, unsigned long)
static void die(const char *m){perror(m);exit(1);}
int main() {
    int fd = open("/dev/mychar", O_RDWR); if (fd < 0) die("open");
    unsigned long sz, dlen, newsz = 1024;
    if (ioctl(fd, MYCHAR_IOC_GET_BUFSZ, &sz) < 0) die("GET_BUFSZ");
    printf("bufsz(before)=%lu\n", sz);
    if (ioctl(fd, MYCHAR_IOC_SET_BUFSZ, &newsz) < 0) die("SET_BUFSZ");
    if (ioctl(fd, MYCHAR_IOC_GET_BUFSZ, &sz) < 0) die("GET_BUFSZ#2");
    printf("bufsz(after)=%lu\n", sz);
    const char *msg = "hello-ioctl";
    if (write(fd, msg, strlen(msg)) < 0) die("write");
    if (ioctl(fd, MYCHAR_IOC_GET_DLEN, &dlen) < 0) die("GET_DLEN");
    printf("data_len=%lu\n", dlen);
    if (ioctl(fd, MYCHAR_IOC_CLEAR) < 0) die("CLEAR");
    if (ioctl(fd, MYCHAR_IOC_GET_DLEN, &dlen) < 0) die("GET_DLEN#2");
    printf("data_len(after clear)=%lu\n", dlen);
    close(fd); return 0;
}

