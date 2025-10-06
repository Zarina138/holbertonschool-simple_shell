#include "shell.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern char **environ;

/**
 * find_command - Finds the full path of a command using PATH
 * @command: Command to locate
 * Return: Full path (malloc’d) if found, otherwise NULL
 */
char *find_command_in_path(char *command)
{
    char *path_env = NULL;
    char *path_copy, *dir;
    char *fullpath;
    int i;

    /* Find PATH variable in environment */
    for (i = 0; environ[i]; i++)
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
        {
            path_env = environ[i] + 5;
            break;
        }
    }

    /* If PATH not found or command includes '/', test it directly */
    if (!path_env || strchr(command, '/'))
    {
        fullpath = strdup(command);
        if (!fullpath)
        {
            perror("strdup");
            exit(1);
        }
        if (access(fullpath, X_OK) == 0)
            return fullpath;
        free(fullpath);
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
    while (dir)
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

