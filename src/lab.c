#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lab.h"

char *get_prompt(const char *env) {
    return '\0';
}

int change_dir(char **dir) {
    return 0;
}

char **cmd_parse(char const *line) {
    return '\0';
}

void cmd_free(char ** line) {
    return;
}

char *trim_white(char *line) {
    return '\0';
}

bool do_builtin(struct shell *sh, char **argv) {
    return false;
}

void sh_init(struct shell *sh) {
    return;
}

void sh_destroy(struct shell *sh) {
    return;
}

void parse_args(int argc, char **argv) {
  int opt;
  while ((opt = getopt(argc, argv, "v")) != -1) {
        switch (opt) {
            case 'v':
                printf("Shell Version: %d.%d\n", lab_VERSION_MAJOR, lab_VERSION_MINOR);
            exit(EXIT_SUCCESS);
            default:
                fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
}

