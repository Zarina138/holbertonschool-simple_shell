#include "shell.h"

extern char **environ;

char *_strdup(const char *s)
{
    char *dup;
    size_t len = strlen(s) + 1;

    dup = malloc(len);
    if (!dup)
        return NULL;
    memcpy(dup, s, len);
    return dup;
}

char *find_command(char *cmd)
{
    char *path_env = NULL, *path_copy = NULL, *dir;
    char full_path[1024];
    char *result = NULL;
    int i;

  
    if (access(cmd, X_OK) == 0)
        return _strdup(cmd);


    for (i = 0; environ[i]; i++)
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
        {
            path_env = environ[i] + 5;
            break;
        }
    }

    if (!path_env || strlen(path_env) == 0)
        return NULL;

    path_copy = _strdup(path_env);
    if (!path_copy)
        return NULL;

    dir = strtok(path_copy, ":");
    while (dir)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);

        if (access(full_path, X_OK) == 0)
        {
            result = _strdup(full_path);
            break;
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return result;
}
