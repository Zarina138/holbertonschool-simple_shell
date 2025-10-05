#include "shell.h"

/**
 * find_command - find executable full path
 * @cmd: command name
 * @envp: environment variables
 * Return: full path string or NULL if not found
 */
char *find_command(char *cmd, char **envp)
{
    char *path_env = NULL, *path_copy, *dir, *fullpath;
    int i;

    if (!cmd)
        return (NULL);

    /* find PATH in envp */
    for (i = 0; envp[i]; i++)
    {
        if (strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_env = envp[i] + 5;
            break;
        }
    }

    /* if no PATH or cmd has '/' */
    if (!path_env || strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (strdup(cmd));
        return (NULL);
    }

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

