#ifndef NK68_CLI_H
#define NK68_CLI_H

#include "nk68.h"

enum cli_action {
    CLI_ACTION_SET,
    CLI_ACTION_GET,
    CLI_ACTION_HELP
};

struct cli_options {
    enum cli_action action;
    struct nk68_settings settings;
};

int cli_parse(int argc, char **argv, struct cli_options *options);
void cli_print_usage(const char *program, int stream);

#endif
