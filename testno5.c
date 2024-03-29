int main(int argc, char *argv[])
{
  char *cmd = NULL;
  int i = 0, j = 0;
  size_t size = 0;
  char *arr[80];//you can use dynamically allocate in heap
  
  char *token;
  
  while (1)
  {
    write(STDOUT_FILENO, "#cisfun$ ", 9);
    cmd = _getline();
      
    token = _strtok(cmd, " \n");
    while (token != NULL)
    {
      arr[i] = token;
      i++;
      token = _strtok(NULL, " \n");
    }
    arr[i] = NULL;

    if (i > 0)
    {
      if (_strcmp(arr[0], "exit") == 0)
      {
        free(cmd);
        exit(0);
      }
      else if (_strcmp(arr[0], "setenv") == 0)
      {
        if (i < 3) 
        {
          printf(" ");
        } 
        else 
        {
          _setenv(arr[1], arr[2], 1);
        }
      }
      else if (_strcmp(arr[0], "unsetenv") == 0)
      {
        if (i < 2)
        {
          printf(" "); 
        }
        else
        {
          _unsetenv(arr[1]);
        }
      }
      else
      {
        pid_t pid;
        pid = fork();

        if (pid == 0)
        {
          char *_env = _getenv("PATH");
          char *path = _strtok(_env, ":");

          while (path != NULL)
          {
            char buffer[80];
            snprintf(buffer, sizeof(buffer), "%s/%s", path, arr[0]);
            execve(buffer, arr, environ);
            path = _strtok(NULL, ":");
          }
          execvp(arr[0], arr);
          perror("./main");
          exit(1);
        }
        else
        {
          waitpid(pid, NULL, 0);
        }
      }
    }

    i = 0;
    size = 0;
    free(cmd);
  }
  
  return (0);
}
