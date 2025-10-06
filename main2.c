#include "shell.h"

/**
 * main - entry point
 * Return: 0 on success
 */
int main(void)
{
	return (prompt_loop());
}

/**
 * prompt_loop - handles input loop
 * Return: 0 on success
 */
int prompt_loop(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char **args;
	int status = 0;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, ":) ", 3);

		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			if (feof(stdin))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		args = split_line(line);
		if (!args || !args[0])
		{
			free(args);
			continue;
		}

		if (strcmp(args[0], "exit") == 0)
		{
			free(args);
			break;
		}

		if (strcmp(args[0], "cd") == 0)
		{
			if (args[1] == NULL)
				args[1] = getenv("HOME");
			if (chdir(args[1]) == -1)
				perror("cd");
			free(args);
			continue;
		}

		status = execute_command(args);
		free(args);
		(void)status;
	}

	free(line);
	return (0);
}

/**
 * split_line - tokenizes a string into arguments
 * @line: command line input
 *
 * Return: pointer to array of tokens
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
 * execute_command - handles PATH search, fork, exec, wait
 * @args: argument vector
 * Return: status code
 */
int execute_command(char **args)
{
	pid_t pid;
	int status;
	char *path;

	path = find_command_in_path(args[0]);
	if (!path)
	{
		fprintf(stderr, "%s: not found\n", args[0]);
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
         
