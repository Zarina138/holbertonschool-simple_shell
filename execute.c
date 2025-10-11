#include "shell.h"
#include <sys/wait.h>

/**
 * execute_command - runs a command using fork + execve
 * @args: arguments array
 * @shell_name: program name
 * Return: 1 to continue, 0 to exit
 */
int execute_command(char **args, char *shell_name)
{
	pid_t pid;
	int status;
	char *cmd_path;

	if (!args || !args[0])
		return (1);

	/* Built-in: exit */
	if (strcmp(args[0], "exit") == 0)
		return (0);

	cmd_path = find_command_in_path(args[0]);
	if (!cmd_path)
	{
		dprintf(STDERR_FILENO, "%s: 1: %s: not found\n", shell_name, args[0]);
		return (1);
	}

	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd_path, args, environ) == -1)
			perror(shell_name);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork");
	}
	else
	{
		waitpid(pid, &status, 0);
	}
	free(cmd_path);
	return (1);
}
