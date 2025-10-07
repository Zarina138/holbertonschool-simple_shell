#include "shell.h"

/**
 * split_line - Tokenizes a string into arguments
 * @line: Command line input
 * Return: Pointer to array of tokens
 */
char **split_line(char *line)
{
	int bufsize = 64, pos = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if (!tokens)
		return (NULL);

	token = strtok(line, " \t\r\n\a");
	while (token)
	{
		tokens[pos++] = token;
		if (pos >= bufsize)
		{
			bufsize += 64;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
				return (NULL);
		}
		token = strtok(NULL, " \t\r\n\a");
	}
	tokens[pos] = NULL;
	return (tokens);
}

/**
 * execute_command - Executes a command using PATH, fork, execve
 * @args: Argument vector
 * Return: Status code
 */
int execute_command(char **args)
{
	pid_t pid;
	int status;
	char *path;

	path = find_command_in_path(args[0]);
	if (!path)
	{
		fprintf(stderr, "./hsh: %s: not found\n", args[0]);
		return (-1);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(path);
		return (-1);
	}
	if (pid == 0)
	{
		execve(path, args, environ);
		perror("execve");
		free(path);
		_exit(127);
	}
	free(path);
	waitpid(pid, &status, 0);
	return (WIFEXITED(status) ? WEXITSTATUS(status) : -1);
}

