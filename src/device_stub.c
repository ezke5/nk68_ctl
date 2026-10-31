#include "nk68.h"

#include <errno.h>

int nk68_hid_get_feature(int fd, unsigned char *report, size_t length)
{
    (void)fd;
    (void)report;
    (void)length;
    errno = ENOTSUP;
    return -1;
}

int nk68_hid_set_feature(int fd, unsigned char *report, size_t length)
{
    (void)fd;
    (void)report;
    (void)length;
    errno = ENOTSUP;
    return -1;
}

int nk68_find_device(char *path, size_t path_len)
{
    (void)path;
    (void)path_len;
    errno = ENOTSUP;
    return -1;
}
