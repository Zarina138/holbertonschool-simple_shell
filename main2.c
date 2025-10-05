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

	/* Print prompt */
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
 * parse_line - trim newline and spaces; only single-word commands allowed
 * @line: input line
 * Return: pointer to cleaned command or NULL if empty/invalid
 */
char *parse_line(char *line)
{
	char *start, *end;

	if (line == NULL)
		return (NULL);

	end = line + strlen(line) - 1;
	while (end >= line && (*end == '\n' || *end == '\r'))
		*end-- = '\0';

	start = line;
	while (*start == ' ' || *start == '\t')
		start++;

	if (*start == '\0')
		return (NULL);

	end = start + strlen(start) - 1;
	while (end > start && (*end == ' ' || *end == '\t'))
		*end-- = '\0';

	if (strchr(start, ' ') || strchr(start, '\t'))
		return (NULL);

	return (start);
}

/**
 * execute_cmd - fork and execve the given command (no args)
 * @cmd: command path (single word)
 * Return: 0 on success, -1 on failure
 */
int execute_cmd(char *cmd)
{
	pid_t pid;
	int status;
	char *argv_exec[2];

	if (cmd == NULL)
		return (-1);

	argv_exec[0] = cmd;
	argv_exec[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}

	if (pid == 0)
	{
		/* child process */
		if (execve(cmd, argv_exec, environ) == -1)
		{
			dprintf(STDERR_FILENO, "simple_shell: %s: %s\n",
				cmd, strerror(errno));
			_exit(EXIT_FAILURE);
		}
	}
	else
	{
		/* parent process */
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
	char *cmd = NULL;

	while (1)
	{
		line = prompt_and_read();
		if (line == NULL)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		cmd = parse_line(line);
		if (cmd == NULL)
		{
			free(line);
			continue;
		}

		execute_cmd(cmd);
		free(line);
	}

	return (EXIT_SUCCESS);
}

