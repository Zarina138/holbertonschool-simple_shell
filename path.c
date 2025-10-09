#include "shell.h"

/**
 * get_env_value - returns the value of an environment variable
 */
char *get_env_value(const char *name)
{
	int i;
	size_t len;

	if (!name || !environ)
		return (NULL);
	len = strlen(name);

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
			return (environ[i] + len + 1);
	}
	return (NULL);
}

/**
 * find_command_in_path - search PATH directories for a command
 */
char *find_command_in_path(const char *command)
{
	char *path_value, *path_copy, *dir, *full_path;
	size_t cmd_len, dir_len;

	if (!command)
		return (NULL);
	if (strchr(command, '/'))
		return (strdup(command));

	path_value = get_env_value("PATH");
	if (!path_value)
		return (NULL);

	path_copy = strdup(path_value);
	if (!path_copy)
		return (NULL);

	cmd_len = strlen(command);
	dir = strtok(path_copy, ":");

	while (dir)
	{
		dir_len = strlen(dir);
		full_path = malloc(dir_len + cmd_len + 2);
		if (!full_path)
			break;

		strcpy(full_path, dir);
		full_path[dir_len] = '/';
		strcpy(full_path + dir_len + 1, command);

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
