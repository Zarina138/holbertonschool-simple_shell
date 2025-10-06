#include "shell.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern char **environ;

/**
 * find_command_in_path - Finds the full path of a command using PATH
 * @command: Command to locate
 * Return: Full path (malloc’d) if found, otherwise NULL
 */
char *find_command_in_path(char *command)
{
    char *path_env = NULL;
    char *path_copy;
    char *dir;
    char *fullpath;
    int i;

    /* Try to get PATH from environ or getenv */
    for (i = 0; environ && environ[i]; i++)
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
        {
            path_env = environ[i] + 5;
            break;
        }
    }

    if (!path_env)
        path_env = getenv("PATH");

    /* If PATH not found or command includes '/', test directly */
    if (!path_env || strchr(command, '/') != NULL)
    {
        if (access(command, X_OK) == 0)
        {
            fullpath = strdup(command);
            if (!fullpath)
            {
                perror("strdup");
                exit(1);
            }
            return fullpath;
        }
        return NULL;
    }

    /* Search each directory in PATH */
    path_copy = strdup(path_env);
    if (!path_copy)
    {
        perror("strdup");
        exit(1);
    }

    dir = strtok(path_copy, ":");
    while (dir != NULL)
    {
        fullpath = malloc(strlen(dir) + strlen(command) + 2);
        if (!fullpath)
        {
            perror("malloc");
            free(path_copy);
            exit(1);
        }

        sprintf(fullpath, "%s/%s", dir, command);

        if (access(fullpath, X_OK) == 0)
        {
            free(path_copy);
            return fullpath;
        }

        free(fullpath);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

