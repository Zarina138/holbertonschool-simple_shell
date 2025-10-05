#include "shell.h"

/**
 * prompt_and_read - print prompt and read a line from stdin
 * Return: pointer to the line (caller must free) or NULL on EOF/error
 */
char *prompt_and_read(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "$ ", 2);

	nread = getline(&line, &len, stdin);
	if (nread == -1)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

/**
 * split_line - split input line into tokens (by space/tab)
 * @line: input line
 * Return: NULL-terminated array of tokens, or NULL on error
 */
char **split_line(char *line)
{
	char **tokens = NULL;
	char *token;
	size_t bufsize = 64, i = 0;

	tokens = malloc(sizeof(char *) * bufsize);
	if (!tokens)
		return (NULL);

	token = strtok(line, " \t\r\n");
	while (token)
	{
		tokens[i++] = token;
		if (i >= bufsize)
			break;
		token = strtok(NULL, " \t\r\n");
	}
	tokens[i] = NULL;
	return (tokens);
}

/**
 * execute_cmd - fork and execve the given command with args
 * @argv_exec: array of arguments (argv[0] = command)
 * Return: 0 on success, -1 on failure
 */
int execute_cmd(char **argv_exec)
{
	pid_t pid;
	int status;

	if (argv_exec == NULL || argv_exec[0] == NULL)
		return (-1);

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}

	if (pid == 0)
	{
		execve(argv_exec[0], argv_exec, environ);
		dprintf(STDERR_FILENO, "./hsh: %s: not found\n", argv_exec[0]);
		_exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, &status, 0);

	return (0);
}

/**
 * main - entry point for simple shell
 * Return: EXIT_SUCCESS
 */
int main(void)
{
	char *line = NULL;
	char **argv = NULL;

	while (1)
	{
		line = prompt_and_read();
		if (line == NULL)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		argv = split_line(line);
		if (argv && argv[0])
			execute_cmd(argv);

		free(argv);
		free(line);
	}
	return (EXIT_SUCCESS);
}

