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
 * parse_line - split line into words (program + arguments)
 * @line: input line
 * Return: NULL-terminated array of tokens, or NULL if empty
 */
char **parse_line(char *line)
{
	char **argv = NULL;
	char *token;
	int bufsize = 8, i = 0;

	if (line == NULL)
		return (NULL);

	argv = malloc(sizeof(char *) * bufsize);
	if (!argv)
		return (NULL);

	token = strtok(line, " \t\n");
	while (token != NULL)
	{
		argv[i++] = token;
		if (i >= bufsize)
		{
			bufsize += 8;
			argv = realloc(argv, sizeof(char *) * bufsize);
			if (!argv)
				return (NULL);
		}
		token = strtok(NULL, " \t\n");
	}
	argv[i] = NULL;

	if (i == 0)
	{
		free(argv);
		return (NULL);
	}

	return (argv);
}

/**
 * execute_cmd - fork and execve the given command (with args)
 * @argv: argument vector
 * Return: 0 on success, -1 on failure
 */
int execute_cmd(char **argv)
{
	pid_t pid;
	int status;

	if (argv == NULL || argv[0] == NULL)
		return (-1);

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}

	if (pid == 0)
	{
		if (execve(argv[0], argv, environ) == -1)
		{
			dprintf(STDERR_FILENO, "simple_shell: %s: %s\n",
				argv[0], strerror(errno));
			_exit(EXIT_FAILURE);
		}
	}
	else
		waitpid(pid, &status, 0);

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
		if (line == NULL)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		argv = parse_line(line);
		if (argv == NULL)
		{
			free(line);
			continue;
		}

		execute_cmd(argv);

		free(argv);
		free(line);
	}

	return (EXIT_SUCCESS);
}

