#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

extern char **environ;

char *_getline()
{
  char c ;
  int i = 0;
  int size = 80;

  char *buffer = malloc (size * sizeof(char));
  if (buffer == NULL)
  {
    return (NULL);
  }

  while ((c = getchar()) != '\n' && c != EOF)
    {
      buffer[i++] = c;

      if (i == size)
      {
        size *= 2;  // increase buffer size by doubling
        char *temp = realloc (buffer ,size * sizeof(char));

        if (temp == NULL)
        {
         free (buffer);
          return (NULL);
        }
        buffer = temp;  // update buffer to point to the newly allocated memory
      }
    }
  buffer[i] = '\0';
  return (buffer);
}

int main (int argc ,char *argv[])
{
  char *token;
  char *why;
  
  while (1)
    {
      printf("#cisfun$ ");
      why = _getline();
      
      if(feof(stdin) )
      {
        free(why);  // free allocated memory before exiting
        exit (1);
      }
      token = strtok (why, " "); 
      if (strcmp(token,"exit") == 0)  // check the first token, not the whole string
      {
        free(why);  // free allocated memory before exiting
        break;
      }

      pid_t pid = fork();

      if (pid < 0)
      {
        perror ("fork");
        free(why);  // free allocated memory before exiting
        continue;
      }
      else if (pid == 0)
      {
        char *arg[] = {token, NULL};  // use the first token as the command

        execvp (arg[0],arg);  // use execvp to search for the command in PATH
        perror ("execvp");  // print error message
        free(why);  // free allocated memory before exiting
        exit (1);
      }
      else
      {
        wait (NULL);
      }
      
    }
  free (why);
  return (0);
}
