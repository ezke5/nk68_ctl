#ifndef NK68_H
#define NK68_H

#include <stddef.h>

#define NK68_MODE_MIN 1
#define NK68_MODE_MAX 13
#define NK68_SPEED_MIN 1
#define NK68_SPEED_MAX 5
#define NK68_BRIGHTNESS_MIN 0
#define NK68_BRIGHTNESS_MAX 5

struct nk68_settings {
    int mode;
    int speed;
    int brightness;
};

const char *nk68_mode_name(int mode);
int nk68_validate_settings(const struct nk68_settings *settings);
int nk68_find_device(char *path, size_t path_len);
int nk68_hid_get_feature(int fd, unsigned char *report, size_t length);
int nk68_hid_set_feature(int fd, unsigned char *report, size_t length);
int nk68_get_status(int fd);
int nk68_set_settings(int fd, const struct nk68_settings *settings);

#endif
