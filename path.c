#include "shell.h"

/**
 * find_command - find the full path of a command in PATH
 * @cmd: command name
 *
 * Return: malloc'd string with full path or NULL if not found
 */
char *find_command(char *cmd)
{
    char *path_env, *path_copy, *dir, *fullpath;

    if (!cmd)
        return (NULL);

    /* If cmd already includes '/' check directly */
    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (strdup(cmd));
        return (NULL);
    }

    path_env = getenv("PATH");
    if (!path_env)
        return (NULL);

    path_copy = strdup(path_env);
    if (!path_copy)
        return (NULL);

    dir = strtok(path_copy, ":");
    while (dir)
    {
        fullpath = malloc(strlen(dir) + strlen(cmd) + 2);
        if (!fullpath)
        {
            free(path_copy);
            return (NULL);
        }

        sprintf(fullpath, "%s/%s", dir, cmd);

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

