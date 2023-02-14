#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  char *cmd = NULL;
  size_t size = 0;
  int i = 0;
  char *args[80];
  char *token1;
  char *s = NULL;

  while (1)
  {
    printf("#cisfun$ ");
    getline(&cmd, &size, stdin);

    if (feof(stdin))
    {
      break;
    }

    char *token = strtok(cmd, " \n");
    if (strcmp(token, "exit") == 0)
    {
      break;
    }
    if (strcmp(token, "clear") == 0)
    {
      system("clear");
      continue;
    }
    
    while (token != NULL)
    {
      if (i == 1)
      {
        token1 = token;
      }
      args[i++] = token;
      token = strtok(NULL, " \n");
    }
    args[i] = NULL;

    if (strcmp(args[0], "cd") == 0)
    {
      chdir(token1);
      continue;
    }
    if (strcmp(args[0], "pwd") == 0)
    {
      char s[80];
      getcwd(s, 80);
      printf("%s\n", s);
      continue;
    }
    if (strcmp(args[0], "env") == 0)
    {
      int i;
      for (i = 0; environ[i] != NULL; i++)
      {
        printf("%s\n", environ[i]);
      }
      continue;
    }
    
    pid_t pid;
    pid = fork();
    
    if (pid == 0)
    {
      char *env = getenv("PATH");
      char *path = strtok(env, ":");
      
      while (path != NULL)
        {
          char buffer[100];
        snprintf(buffer, sizeof(buffer), "%s/%s", path, args[0]);

        execve(buffer, args, environ);
        path = strtok(NULL, ":");
        }
      perror("./main");
      exit(1);
    }
    else
    {
      waitpid(pid, NULL, 0);
    }

    i = 0;
    free(cmd);
    cmd = NULL;
    size = 0;
    free(s);
  }

  free(cmd);
  return 0;
}
