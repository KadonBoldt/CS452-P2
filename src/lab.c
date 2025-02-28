#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lab.h"

char *get_prompt(const char *env) {
    char *prompt = getenv(env);
    return prompt ? prompt : strdup("shell>");
}

int change_dir(char **args) {
    if (!args[1]) {
        const char *home = getenv("HOME");
        if (!home) {
            struct passwd *pw = getpwuid(getuid());
            home = pw ? pw->pw_dir : "";
        }
        return chdir(home);
    }
    return chdir(args[1]);
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
    if (!argv || !argv[0]) return false;
    if (strcmp(argv[0], "exit") == 0) {
        sh_destroy(sh);
        exit(0);
    } else if (strcmp(argv[0], "cd") == 0) {
        size_t cd_result = change_dir(argv);
      	if (cd_result != 0) {
            perror("cd failed");
        }
        return cd_result == 0;
    } else if (strcmp(argv[0], "history") == 0) {
        HIST_ENTRY **history = history_list();
        if (history) {
            for (int i = 0; history[i]; i++) {
                printf("%d: %s\n", i + 1, history[i]->line);
            }
        }
        return true;
    }
    return false;
}

void sh_init(struct shell *sh) {
  	sh->shell_terminal = STDIN_FILENO;
    sh->shell_is_interactive = isatty(sh->shell_terminal);
    if (sh->shell_is_interactive) {
      	while (tcgetpgrp(sh->shell_terminal) != (sh->shell_pgid = getpgrp())) {
          	kill(-sh->shell_pgid, SIGTTIN);
        }
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
      	sh->shell_pgid = getpid();
        setpgid(sh->shell_pgid, sh->shell_pgid);
        tcsetpgrp(sh->shell_terminal, sh->shell_pgid);
        tcgetattr(sh->shell_terminal, &sh->shell_tmodes);
    }
    sh->prompt = get_prompt("MY_PROMPT");
}

void sh_destroy(struct shell *sh) {
    free(sh->prompt);
    sh->prompt = NULL;
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

