#include "shell.h"

/**
 * prompt_and_read - print prompt and read a line from stdin using getline
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
 * split_line - splits a line into tokens (space/tab separated)
 * @line: input line
 * Return: NULL-terminated array of args (caller must free)
 */
char **split_line(char *line)
{
	char *token, **argv = NULL;
	size_t size = 0, i = 0;

	token = strtok(line, " \t\n");
	while (token)
	{
		argv = realloc(argv, sizeof(char *) * (i + 2));
		if (!argv)
			return (NULL);
		argv[i++] = token;
		token = strtok(NULL, " \t\n");
	}
	if (argv)
		argv[i] = NULL;
	return (argv);
}

/**
 * execute_cmd - fork and execve the given command with args
 * @argv_exec: command and args
 * Return: 0 on success, -1 on failure
 */
int execute_cmd(char **argv_exec)
{
	pid_t pid;
	int status;

	if (!argv_exec || !argv_exec[0])
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
		perror(argv_exec[0]);
		_exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
	}
	return (0);
}

/**
 * main - entry point for the simple shell
 * Return: EXIT_SUCCESS
 */
int main(void)
{
	char *line = NULL;
	char **argv = NULL;

	while (1)
	{
		line = prompt_and_read();
		if (!line)
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

