#include <stdio.h>
#include <stdlib.h>
#include "main.h"
/**
 * execute_command - func for executing command
 * @args: arguments to path
 * @path: path
 */
void execute_command(char **args, char *path)
{
	pid_t pid;
	int status;

	pid = fork();

	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (execve(path, args, environ) == -1)
		{
			free(path);
			fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
			exit(EXIT_FAILURE);
		}
	}

	else
	{
		if (wait(&status) == -1)
		{
			free(path);
			perror("wait failed");
			exit(EXIT_FAILURE);
		}
	}
}
