#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

extern char **environ;

char *_strtok(char *str, const char *delim)
{
    static char *last;
    char *nextp;

    if (str != NULL)
    {
        last = str;
    }

    if (*last == '\0')
    {
        return NULL;
    }

    char *p = strpbrk(last, delim);

    if (p == NULL)
    {
        nextp = strchr(last, '\0');
    }
    else
    {
        *p = '\0';
        nextp = p + 1;
    }

    char *token = last;
    last = nextp;

    return token;
}

char *_getline()
{
    char c;
    int i = 0;
    int size = 80;

    char *buffer = malloc(size * sizeof(char));
    if (buffer == NULL)
    {
        return NULL;
    }

    while ((c = getchar()) != '\n' && c != EOF)
    {
        buffer[i++] = c;

        if (i == size)
        {
            size *= 2;
            char *temp = realloc(buffer, size * sizeof(char));

            if (temp == NULL)
            {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }
    }
    buffer[i] = '\0';
    return buffer;
}

int main(int argc, char *argv[])
{
    char *token;
    char *why;
    char *token1;
    int i = 0;

    while (1)
    {
        printf("#cisfun$ ");
        why = _getline();

        if (feof(stdin))
        {
            free(why);
            exit(1);
        }

        token = _strtok(why, " ");

        if (strcmp(token, "exit") == 0)
        {
            free(why);
            break;
        }

        i = 0;
        while (token != NULL && i < 2)
        {
            if (i == 0)
            {
                token1 = token;
            }
            i++;
            printf("%s\n", token);
            token = _strtok(NULL, " ");
        }

        if (strcmp(token1, "cd") == 0)
        {
            chdir(token);
        }
        else
        {
            pid_t pid = fork();

            if (pid < 0)
            {
                perror("fork");
                free(why);
                continue;
            }
            else if (pid == 0)
            {
                char *arg[] = {token1, token, NULL};

                execve(arg[0], arg, environ);
                perror("execve");
                free(why);
                exit(1);
            }
            else
            {
                wait(NULL);
            }
        }

        free(why);
    }

    return 0;
}
