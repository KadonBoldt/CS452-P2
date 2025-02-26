#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lab.h"

char *get_prompt(const char *env) {
    char *prompt = getenv(env);
    if (!prompt) {
        return strdup("shell>");
    }
    return strdup(prompt);
}

int change_dir(char **dir) {
    return 0;
}

char **cmd_parse(const char *line) {
    size_t max_args = sysconf(_SC_ARG_MAX);
    char **args = malloc(max_args * sizeof(char *));
    if (!args) return NULL;
    size_t i = 0;
    char *line_cpy = strdup(line);
    char *token = strtok(line_cpy, " ");
    while (token && i < max_args - 1) {
        args[i++] = strdup(token);
        token = strtok(NULL, " ");
    }
    free(line_cpy);
    args[i] = NULL;
    return args;
}

void cmd_free(char **cmd) {
    if (!cmd) return;
    for (size_t i = 0; cmd[i] != NULL; i++) {
        free(cmd[i]);
    }
    free(cmd);
}

char *trim_white(char *line) {
    while (isspace((unsigned char)*line)) line++;
    if (!*line) return line;
    char *end = line + strlen(line) - 1;
    while (end > line && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return line;
}

bool do_builtin(struct shell *sh, char **argv) {
    return false;
}

void sh_init(struct shell *sh) {
    sh = malloc(sizeof(struct shell));
    sh->prompt = "shell>";
}

void sh_destroy(struct shell *sh) {
    free(sh->prompt);
    free(sh);
}

void parse_args(int argc, char **argv) {
  int opt;
  while ((opt = getopt(argc, argv, "v")) != -1) {
        switch (opt) {
            case 'v':
                printf("Simple Shell Version %d.%d\n", lab_VERSION_MAJOR, lab_VERSION_MINOR);
                exit(EXIT_SUCCESS);
        }
    }
}

