#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

int main()
{
    char* line = NULL;
    char* command;
    size_t len = 0;
  
    while(1) 
    {
        printf("#cisfun$ ");
        getline(&line, &len, stdin);
        
        if (feof(stdin))
        {
            printf("\n");
            break;
        }
        
        command = strtok(line, " \n");

        if (command == NULL)
        {
            continue;
        }

        pid_t pid = fork();
        
        if (pid < 0) 
        {
            fprintf(stderr, "Fork failed\n");
            continue;
        } 
        else if (pid == 0) 
        {
            // Child process
            char* args[] = {command, NULL};
            execve(command, args, environ);
            fprintf(stderr, "Command not found\n");
            exit(1);
        } 
        else 
        {
            // Parent process
            wait(NULL);
        }
    }

    return 0;
}
