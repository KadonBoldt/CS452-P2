#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lab.h"

void parse_args(int argc, char **argv) {
    while ((int opt = getopt(argc, argv, "v")) != -1) {
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