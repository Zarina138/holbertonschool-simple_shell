#include "shell.h"

/**
 * main - Entry point of the shell
 * Return: 0 on success
 */
int main(void)
{
	return (prompt_loop());
}

/**
 * prompt_loop - Repeatedly displays prompt and executes commands
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
