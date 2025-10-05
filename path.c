#include "shell.h"

/**
 * find_command - find full path of a command using PATH env
 * @cmd: command name
 * Return: full path (malloced) or NULL if not found
 */
char *find_command(char *cmd)
{
	char *path, *path_copy, *dir;
	char full_path[1024];
	int len;

	if (access(cmd, X_OK) == 0)
		return strdup(cmd);

	path = getenv("PATH");
	if (!path)
		return NULL;

	path_copy = strdup(path);
	dir = strtok(path_copy, ":");

	while (dir)
	{
		len = snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
		if (len < (int)sizeof(full_path) && access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return strdup(full_path);
		}
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return NULL;
}

