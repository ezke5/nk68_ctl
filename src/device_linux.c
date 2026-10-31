#define _GNU_SOURCE
#include "nk68.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <linux/hidraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define TARGET_VID 0x258A
#define TARGET_PID 0x013B

int nk68_hid_get_feature(int fd, unsigned char *report, size_t length)
{
    return ioctl(fd, HIDIOCGFEATURE(length), report);
}

int nk68_hid_set_feature(int fd, unsigned char *report, size_t length)
{
    return ioctl(fd, HIDIOCSFEATURE(length), report);
}

int nk68_find_device(char *path, size_t path_len)
{
    DIR *dir;
    struct dirent *entry;
    int found = -1;

    if (!path || path_len == 0)
        return -1;
    dir = opendir("/sys/class/hidraw");
    if (!dir)
        return -1;

    while ((entry = readdir(dir)) != NULL) {
        char uevent_path[PATH_MAX];
        char device_path[PATH_MAX];
        char resolved[PATH_MAX];
        FILE *file;
        char line[256];
        int matched = 0;

        if (strncmp(entry->d_name, "hidraw", 6) != 0)
            continue;
        if (snprintf(uevent_path, sizeof(uevent_path),
                     "/sys/class/hidraw/%s/device/uevent", entry->d_name)
            >= (int)sizeof(uevent_path))
            continue;
        file = fopen(uevent_path, "r");
        if (!file)
            continue;
        while (fgets(line, sizeof(line), file)) {
            unsigned int bus, vid, pid;
            if (sscanf(line, "HID_ID=%x:%x:%x", &bus, &vid, &pid) == 3 &&
                vid == TARGET_VID && pid == TARGET_PID) {
                matched = 1;
                break;
            }
        }
        fclose(file);
        if (!matched)
            continue;
        if (snprintf(device_path, sizeof(device_path),
                     "/sys/class/hidraw/%s/device", entry->d_name)
            >= (int)sizeof(device_path))
            continue;
        if (!realpath(device_path, resolved) || !strstr(resolved, ":1.1"))
            continue;
        if (snprintf(path, path_len, "/dev/%s", entry->d_name) >= (int)path_len)
            break;
        found = 0;
        break;
    }
    closedir(dir);
    return found;
}
