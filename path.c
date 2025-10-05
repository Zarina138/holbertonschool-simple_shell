#include "shell.h"

char *find_command(char *cmd, char **envp)
{
    char *path_env = NULL, *path_copy = NULL, *dir;
    char full_path[1024];
    char *result = NULL;
    int i;

    /* Əgər cmd absolute/relative pathdirsə */
    if (access(cmd, X_OK) == 0)
        return strdup(cmd);

    /* PATH mühit dəyişənini tap */
    for (i = 0; envp[i]; i++)
    {
        if (strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_env = envp[i] + 5;
            break;
        }
    }

    if (!path_env || strlen(path_env) == 0)
        return NULL;

    path_copy = strdup(path_env);
    if (!path_copy)
        return NULL;

    dir = strtok(path_copy, ":");
    while (dir)
    {
        if (dir[strlen(dir) - 1] == '/')
            snprintf(full_path, sizeof(full_path), "%s%s", dir, cmd);
        else
            snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);

        if (access(full_path, X_OK) == 0)
        {
            result = strdup(full_path);
            break;
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return result;
}

