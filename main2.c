#include "shell.h"

/* read a line from stdin */
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

/* split line into arguments */
char **split_line(char *line)
{
	char **argv = NULL;
	char *token;
	size_t bufsize = 64, i = 0;

	argv = malloc(sizeof(char *) * bufsize);
	if (!argv)
		return (NULL);

	token = strtok(line, " \t\r\n");
	while (token)
	{
		argv[i++] = token;
		if (i >= bufsize)
			break;
		token = strtok(NULL, " \t\r\n");
	}
	argv[i] = NULL;
	return (argv);
}

/* find command in PATH */
char *find_command(char *cmd)
{
	char *path_env, *path_dup, *dir;
	char *full_path;
	size_t len;

	if (!cmd)
		return NULL;

	/* if cmd contains '/', assume it's a path */
	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return strdup(cmd);
		return NULL;
	}

	path_env = getenv("PATH");
	if (!path_env)
		return NULL;

	path_dup = strdup(path_env);
	if (!path_dup)
		return NULL;

	dir = strtok(path_dup, ":");
	while (dir)
	{
		len = strlen(dir) + 1 + strlen(cmd) + 1;
		full_path = malloc(len);
		if (!full_path)
		{
			free(path_dup);
			return NULL;
		}
		snprintf(full_path, len, "%s/%s", dir, cmd);

		if (access(full_path, X_OK) == 0)
		{
			free(path_dup);
			return full_path;
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_dup);
	return NULL;
}

/* fork and execve the command with arguments */
int execute_cmd(char **argv_exec)
{
	pid_t pid;
	int status;
	char *cmd_path;

	if (!argv_exec || !argv_exec[0])
		return (-1);

	cmd_path = find_command(argv_exec[0]);
	if (!cmd_path)
	{
		dprintf(STDERR_FILENO, "%s: command not found\n", argv_exec[0]);
		return (-1);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		return (-1);
	}

	if (pid == 0)
	{
		execve(cmd_path, argv_exec, environ);
		dprintf(STDERR_FILENO, "%s: execution failed\n", argv_exec[0]);
		free(cmd_path);
		_exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, &status, 0);

	free(cmd_path);
	return 0;
}

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

