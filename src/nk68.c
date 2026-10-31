#include "nk68.h"

#include <stdio.h>

static const char *const mode_names[] = {
    "Off", "Mode 1", "Mode 2", "Mode 3", "Mode 4", "Mode 5",
    "Mode 6", "Mode 7", "Mode 8", "Mode 9", "Mode 10", "Mode 11",
    "Mode 12"
};

const char *nk68_mode_name(int mode)
{
    if (mode >= 0 && mode <= 12)
        return mode_names[mode];
    return "Custom";
}

int nk68_validate_settings(const struct nk68_settings *settings)
{
    if (!settings)
        return -1;
    return settings->mode >= NK68_MODE_MIN &&
           settings->mode <= NK68_MODE_MAX &&
           settings->speed >= NK68_SPEED_MIN &&
           settings->speed <= NK68_SPEED_MAX &&
           settings->brightness >= NK68_BRIGHTNESS_MIN &&
           settings->brightness <= NK68_BRIGHTNESS_MAX ? 0 : -1;
}

int nk68_get_status(int fd)
{
    unsigned char report[42] = {0};
    int mode;

    report[0] = 0x0a;
    report[1] = 0x06;
    if (nk68_hid_get_feature(fd, report, sizeof(report)) < 0) {
        perror("HIDIOCGFEATURE failed");
        return -1;
    }

    mode = report[2];
    printf("\n=== Ajazz / NACODEX NK68 Current Status ===\n"
           "  Lighting Mode : %d (%s)\n"
           "  Speed         : %d / 5\n"
           "  Brightness    : %d / 5\n"
           "=============================================\n",
           mode, nk68_mode_name(mode), report[3], report[4]);
    return 0;
}

int nk68_set_settings(int fd, const struct nk68_settings *settings)
{
    unsigned char report[42] = {0};

    if (nk68_validate_settings(settings) < 0)
        return -1;
    report[0] = 0x0a;
    report[1] = 0x08;
    report[2] = settings->mode == 13 ? 0x00 : (unsigned char)settings->mode;
    report[3] = (unsigned char)settings->speed;
    report[4] = (unsigned char)settings->brightness;
    report[9] = 0x93;
    report[14] = 0x0a;

    if (nk68_hid_set_feature(fd, report, sizeof(report)) < 0) {
        perror("HIDIOCSFEATURE failed");
        return -1;
    }
    return 0;
}
