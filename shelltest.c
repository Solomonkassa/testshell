#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>

extern char **environ;

char *_getline() {
    int size = 80;
    int c, i = 0;
    char *buffer;

    buffer = malloc(size * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    while ((c = getchar()) != '\n' && c != EOF) {
        buffer[i++] = (char)c;
        if (i == size) {
            size *= 2;
            buffer = realloc(buffer, size * sizeof(char));
            if (buffer == NULL) {
                fprintf(stderr, "Memory reallocation failed.\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    buffer[i] = '\0';
    return buffer;
}

void my_strtok(char *line, char *delimiter, char **tokens, int *ntokens) {
    char *ptr = strtok(line, delimiter);
    while (ptr != NULL) {
        tokens[(*ntokens)++] = ptr;
        ptr = strtok(NULL, delimiter);
    }
}

void execute(char **tokens, int ntokens) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if (strcmp(tokens[0], "mkdir") == 0) {
            if (ntokens != 2) {
                fprintf(stderr, "Usage: mkdir <directory>\n");
                exit(EXIT_FAILURE);
            }
            if (mkdir(tokens[1], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
                perror("mkdir");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(tokens[0], "cd") == 0) {
            if (ntokens != 2) {
                fprintf(stderr, "Usage: cd <directory>\n");
                exit(EXIT_FAILURE);
            }
            if (chdir(tokens[1]) == -1) {
                perror("chdir");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(tokens[0], "pwd") == 0) {
            if (ntokens != 1) {
                fprintf(stderr, "Usage: pwd\n");
                exit(EXIT_FAILURE);
            }
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) == NULL) {
                perror("getcwd");
                exit(EXIT_FAILURE);
            }
            printf("%s\n", cwd);
        } else if (strcmp(tokens[0], "ls") == 0) {
            if (ntokens != 1) {
                fprintf(stderr, "Usage: ls\n");
                exit(EXIT_FAILURE);
            }
            if (execlp("ls", "ls", (char *)NULL) == -1) {
                perror("execlp");
                exit(EXIT_FAILURE);
            }
        } else {
            if (execvp(tokens[0], tokens) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }
    } else {
        wait(NULL);
    }
}

int main(int argc, char *argv[]) {
    char *line;
    char *tokens[100];
    int ntokens = 0;
    const char *delimiter = " \t\n";

    while (1) {
        printf("$ ");
        line = _getline();

        if (feof(stdin) ==
