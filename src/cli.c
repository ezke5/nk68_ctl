#include "cli.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parse_number(const char *value, int *result)
{
    char *end;
    long parsed;

    if (!value || !*value)
        return -1;
    parsed = strtol(value, &end, 10);
    if (*end || parsed < -2147483648L || parsed > 2147483647L)
        return -1;
    *result = (int)parsed;
    return 0;
}

void cli_print_usage(const char *program, int stream)
{
    FILE *out = stream ? stderr : stdout;
    fprintf(out,
            "Ajazz / NACODEX NK68 CLI Controller\n\n"
            "Usage:\n"
            "  %s --get\n"
            "  %s [options]\n"
            "  %s <mode: 1-13> <speed: 1-5> <brightness: 0-5>\n\n"
            "Options:\n"
            "  -g, --get                 Query current settings\n"
            "  -m, --mode <1-13>        Set lighting animation (13 = off)\n"
            "  -s, --speed <1-5>        Animation speed (default: 3)\n"
            "  -b, --brightness <0-5>   Brightness (default: 5)\n"
            "  -h, --help               Show this help\n",
            program, program, program);
}

int cli_parse(int argc, char **argv, struct cli_options *options)
{
    int opt;
    int positional[3];
    int positional_count = 0;
    static const struct option long_options[] = {
        {"get", no_argument, NULL, 'g'},
        {"mode", required_argument, NULL, 'm'},
        {"speed", required_argument, NULL, 's'},
        {"brightness", required_argument, NULL, 'b'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    if (!options)
        return -1;
    options->action = CLI_ACTION_SET;
    options->settings = (struct nk68_settings){-1, 3, 5};
    optind = 1;
    while ((opt = getopt_long(argc, argv, "gm:s:b:h", long_options, NULL)) != -1) {
        int *target = NULL;
        if (opt == 'm') target = &options->settings.mode;
        if (opt == 's') target = &options->settings.speed;
        if (opt == 'b') target = &options->settings.brightness;
        if (target && parse_number(optarg, target) < 0)
            return -1;
        switch (opt) {
        case 'g': options->action = CLI_ACTION_GET; break;
        case 'h': options->action = CLI_ACTION_HELP; return 0;
        case 'm': case 's': case 'b': break;
        default: return -1;
        }
    }
    for (; optind < argc && positional_count < 3; optind++) {
        if (parse_number(argv[optind], &positional[positional_count++]) < 0)
            return -1;
    }
    if (optind != argc)
        return -1;
    if (positional_count) {
        if (positional_count == 1 && positional[0] == 13) {
            options->settings = (struct nk68_settings){13, 1, 0};
        } else if (positional_count != 3) {
            return -1;
        } else {
            options->settings = (struct nk68_settings){
                positional[0], positional[1], positional[2]
            };
        }
    } else if (options->action == CLI_ACTION_SET &&
               options->settings.mode == -1) {
        return -1;
    }
    return 0;
}
