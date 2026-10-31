#include "cli.h"
#include "nk68.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    struct cli_options options;
    char device_path[PATH_MAX];
    int fd;
    int result;

    if (cli_parse(argc, argv, &options) < 0) {
        cli_print_usage(argv[0], 1);
        return 2;
    }
    if (options.action == CLI_ACTION_HELP) {
        cli_print_usage(argv[0], 0);
        return 0;
    }
    if (nk68_find_device(device_path, sizeof(device_path)) < 0) {
        fprintf(stderr,
                "Error: could not locate the NK68 configuration interface (:1.1).\n");
        return 1;
    }
    fd = open(device_path, O_RDWR);
    if (fd < 0) {
        perror("Failed to open hidraw device");
        return 1;
    }
    if (options.action == CLI_ACTION_GET) {
        result = nk68_get_status(fd);
    } else if (nk68_validate_settings(&options.settings) < 0) {
        fprintf(stderr,
                "Error: values out of bounds "
                "(mode: 1-13, speed: 1-5, brightness: 0-5).\n");
        result = -1;
    } else {
        result = nk68_set_settings(fd, &options.settings);
        if (result == 0) {
            if (options.settings.mode == 13)
                puts("Backlight turned OFF.");
            else
                printf("Applied: Mode %d (%s), Speed %d, Brightness %d\n",
                       options.settings.mode,
                       nk68_mode_name(options.settings.mode),
                       options.settings.speed, options.settings.brightness);
        }
    }
    close(fd);
    return result == 0 ? 0 : 1;
}
