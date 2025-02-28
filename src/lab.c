#include <ctype.h>
#include <errno.h>
#include <pwd.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lab.h"

char *get_prompt(const char *env) {
	char *prompt = getenv(env);
	// Return env variable if set, or default if not.
	return prompt ? strdup(prompt) : strdup("Shell > ");
}

int change_dir(char **args) {
    if (!args[1]) {
        const char *home = getenv("HOME");
        if (!home) {
            struct passwd *pw = getpwuid(getuid());
            // Set home as pw_dir it pw exists else NULL.
            home = pw ? pw->pw_dir : NULL;
        }
        return chdir(home);
    }
    return chdir(args[1]);
}

char **cmd_parse(const char *line) {
    size_t max_args = sysconf(_SC_ARG_MAX);
    char **args = malloc(max_args * sizeof(char *));
    if (!args) {
      	perror("Unable to parse shell arguments.");
      	return NULL;
    }
    size_t i = 0;
    char *line_copy = strdup(line);
    char *token = strtok(line_copy, " ");
    while (token && i < max_args - 1) {
        args[i++] = strdup(token);
        token = strtok(NULL, " ");
    }
    free(line_copy);
    args[i] = NULL;
    return args;
}

void cmd_free(char **cmd) {
    if (!cmd) return;
    // Free all cmd arguments.
    for (size_t i = 0; cmd[i] != NULL; i++) {
        free(cmd[i]);
    }
    free(cmd);
}

char *trim_white(char *line) {
  	// Moves pointer to first non-whitespace character.
    while (isspace((unsigned char)*line)) line++;
    if (!*line) return line;
    char *end = line + strlen(line) - 1;
    // Moves end to last non-whitespace character.
    while (end > line && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return line;
}

bool do_builtin(struct shell *sh, char **argv) {
    if (!argv || !argv[0]) return false;
    // exit command.
    if (strcmp(argv[0], "exit") == 0) {
        sh_destroy(sh);
        exit(0);
    }
    // cd command.
    if (strcmp(argv[0], "cd") == 0) {
        size_t cd_result = change_dir(argv);
      	if (cd_result != 0) {
            perror("cd failed");
        }
        return cd_result == 0;
    }
    // history command.
    if (strcmp(argv[0], "history") == 0) {
        HIST_ENTRY **history = history_list();
        if (history) {
            for (int i = 0; history[i]; i++) {
                printf("%d: %s\n", i + history_base, history[i]->line);
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
      	// Loop until process is in the foreground.
      	while (tcgetpgrp(sh->shell_terminal) != (sh->shell_pgid = getpgrp())) {
          	kill(-sh->shell_pgid, SIGTTIN);
        }
        // Ignore signals.
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
      	sh->shell_pgid = getpid();
        if (setpgid(sh->shell_pgid, sh->shell_pgid) < 0) {
          	perror("Could not create process: setpgid failed.");
        }
        if (tcsetpgrp(sh->shell_terminal, sh->shell_pgid) < 0) {
          	perror("Could not create process: tcsetpgrp failed.");
        }
        if (tcgetattr(sh->shell_terminal, &sh->shell_tmodes) < 0) {
          	perror("Could not create process: tcgetattr failed.");
        }
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
          	// Print out version.
            case 'v':
                printf("Simple Shell Version %d.%d\n", lab_VERSION_MAJOR, lab_VERSION_MINOR);
                exit(EXIT_SUCCESS);
        }
    }
}

