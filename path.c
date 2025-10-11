#include "shell.h"

/**
 * get_env_value - returns the value of an environment variable
 * @name: name of the environment variable
 * Return: pointer to the value or NULL
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
 * find_command_in_path - finds the full path of a command
 * @command: command name (e.g. "ls")
 * Return: malloc'd string with full path or NULL if not found
 */
char *find_command_in_path(char *command)
{
	char *path_env, *path_copy, *dir, *full_path;
	size_t cmd_len, dir_len;

	if (!command)
		return (NULL);

	/* if command already has '/' assume it's full path */
	if (strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (strdup(command));
		return (NULL);
	}

	path_env = get_env_value("PATH");
	if (!path_env)
		return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);

	cmd_len = strlen(command);
	dir = strtok(path_copy, ":");
	while (dir)
	{
		dir_len = strlen(dir);
		full_path = malloc(dir_len + cmd_len + 2);
		if (!full_path)
		{
			free(path_copy);
			return (NULL);
		}
		sprintf(full_path, "%s/%s", dir, command);

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
