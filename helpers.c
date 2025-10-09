#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * find_command_in_path - Searches for a command in the system PATH
 * @command: Command name to search for
 *
 * Return: Full path to the command if found, NULL otherwise
 * Caller must free the returned string.
 */
char *find_command_in_path(const char *command)
{
    char *path, *path_copy, *dir;
    struct stat st;
    char tmp[1024];

    path = getenv("PATH");
    if (!path)
        return NULL;

    path_copy = strdup(path);
    if (!path_copy)
        return NULL;

    dir = strtok(path_copy, ":");
    while (dir)
    {
        snprintf(tmp, sizeof(tmp), "%s/%s", dir, command);
        if (stat(tmp, &st) == 0 && (st.st_mode & S_IXUSR))
        {
            free(path_copy);
            return strdup(tmp);
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}
