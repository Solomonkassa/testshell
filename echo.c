#include "shell.h"

/**
 * main - entry point for the shell
 * @argc: argument count
 * @argv: array of arguments
 *
 * Return: 0 on success, non-zero on failure
 */
int main(int argc, char *argv[])
{
	char *line = NULL;
	size_t line_size = 0;
	char *args[MAX_ARGS];

	while (1)
	{
		printf("#cisfun$ ");
		if (getline(&line, &line_size, stdin) == -1)
		{
			break;
		}
		read_command(line, args);
		execute_command(args);
	}

	free(line);
	return (0);
}

/**
 * read_command - reads user input and tokenizes it into arguments
 * @line: user input
 * @args: array of arguments
 *
 * Return: void
 */
void read_command(char *line, char **args)
{
	int i = 0;
	char *token = strtok(line, " \n");
	char exit_str[10];
	char pid_str[10];

	while (token != NULL)
	{
		if (token[0] == '$')
		{
			if (strcmp(token, "$?") == 0)
			{
				printf("%d\n", last_exit_code);
				args[i] = strdup(exit_str);
			}
			else if (strcmp(token, "$$") == 0)
			{
				sprintf(pid_str, "%d", getpid());
				args[i] = strdup(pid_str);
			}
			else if (strcmp(token, "$PATH") == 0)
			{
				printf("%s\n", getenv("PATH"));
				args[i] = NULL;
				return;
			}
			else
			{
				args[i] = strdup(token);
			}
		}
		else
		{
			args[i] = strdup(token);
		}
		i++;
		token = strtok(NULL, " \n");
	}

	args[i] = NULL;
}

/**
 * execute_command - forks and executes the user command
 * @args: array of arguments
 *
 * Return: void
 */
void execute_command(char **args)
{
	if (args[0] == NULL)
	{
		return;
	}

	if (strcmp(args[0], "exit") == 0)
	{
		exit(0);
	}

	pid_t pid = fork();

	if (pid < 0)
	{
		perror(" ");
	}
	else if (pid == 0)
	{
		char *env = getenv("PATH");
		char *path = strtok(env, ":");
		char buffer[MAX_LINE];

		while (path != NULL)
		{
			snprintf(buffer, MAX_LINE, "%s/%s", path, args[0]);
			execve(buffer, args, environ);
			path = strtok(NULL, ":");
		}

		perror(" ");
		exit(1);
	}
	else
	{
		int status;
		wait(&status);
		last_exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
	}
}
