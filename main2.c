#include "shell.h"

/**
 * main - entry point of the shell
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success
 */
int main(int argc, char **argv)
{
	char *line, **args;
	int status = 1;

	(void)argc;

	while (status)
	{
		display_prompt();
		line = read_command();
		if (!line)
			break;

		args = split_line(line);
		status = execute_command(args, argv[0]);

		free(line);
		free_args(args);
	}
	return (0);
}
