#include "shell.h"

/**
 * find_command_in_path - Finds full path of a command using PATH
 * @command: Command to locate
 * Return: Full path (malloc’d) if found, otherwise NULL
 */
char *find_command_in_path(char *command)
{
	char *path_env, *path_copy, *dir, *fullpath;

	path_env = getenv("PATH");
	if (!path_env)
		path_env = "/bin:/usr/bin"; /* Default PATH if not set */

	if (strchr(command, '/') != NULL)
	{
		if (access(command, X_OK) == 0)
			return (strdup(command));
		return (NULL);
	}

	path_copy = strdup(path_env);
	if (!path_copy)
	{
		perror("strdup");
		exit(EXIT_FAILURE);
	}

	dir = strtok(path_copy, ":");
	while (dir)
	{
		fullpath = malloc(strlen(dir) + strlen(command) + 2);
		if (!fullpath)
		{
			perror("malloc");
			free(path_copy);
			exit(EXIT_FAILURE);
		}
		sprintf(fullpath, "%s/%s", dir, command);
		if (access(fullpath, X_OK) == 0)
		{
			free(path_copy);
			return (fullpath);
		}
		free(fullpath);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}
