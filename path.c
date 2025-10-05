#include "shell.h"
/**
 * find_command - locate a command using PATH
 * @cmd: command name
 *
 * Return: malloc'd string with full path if found, or NULL
 */
char *find_command(char *cmd)
{
    if (!cmd)
        return NULL;

    /* absolute or relative path check */
    if (cmd[0] == '/' || cmd[0] == '.') {
        if (access(cmd, X_OK) == 0)
            return strdup(cmd);
        return NULL;
    }

    /* check in PATH */
    char *path_env = getenv("PATH");
    if (!path_env)
        return NULL;

    char *path_copy = strdup(path_env);
    if (!path_copy)
        return NULL;

    char *dir = strtok(path_copy, ":");
    char full_path[1024];

    while (dir != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return strdup(full_path);
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

