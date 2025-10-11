#include "main.h"

/**
 * find_in_path - searches for command in PATH
 * @cmd: command name
 * Return: full path string or NULL if not found
 */
char *find_in_path(char *cmd)
{
    char *path, *path_copy, *token, full_path[1024];
    struct stat st;

    if (cmd == NULL)
        return (NULL);

    if (cmd[0] == '/' || cmd[0] == '.')
    {
        if (stat(cmd, &st) == 0)
            return (cmd);
        else
            return (NULL);
    }

    path = getenv("PATH");
    if (!path)
        return (NULL);

    path_copy = strdup(path);
    token = strtok(path_copy, ":");
    while (token)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", token, cmd);
        if (stat(full_path, &st) == 0)
        {
            free(path_copy);
            return (strdup(full_path));
        }
        token = strtok(NULL, ":");
    }

    free(path_copy);
    return (NULL);
}
