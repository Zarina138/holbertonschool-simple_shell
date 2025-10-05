#include "shell.h"

/**
 * find_command - search for a command in PATH directories
 * @cmd: command name (e.g., "ls")
 * @envp: environment variables
 *
 * Return: malloc'd full path if found, or NULL if not found
 */
char *find_command(char *cmd, char **envp)
{
    char *path_env = NULL, *path_copy, *dir;
    char full_path[1024];
    char *result = NULL;
    int i = 0;

    /* PATH-i tapmaq */
    while (envp && envp[i])
    {
        if (strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_env = envp[i] + 5;
            break;
        }
        i++;
    }

    /* Əgər PATH boşdursa və ya tapılmadısa */
    if (!path_env || strlen(path_env) == 0)
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
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);

        if (access(full_path, X_OK) == 0)
        {
            result = strdup(full_path);
            break;
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return (result);
}

