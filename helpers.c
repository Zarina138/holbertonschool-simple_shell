#include "shell.h"

/**
 * find_command_in_path - Searches for a command in the system PATH
 * @command: Command name to search for
 *
 * Return: Full path to the command if found, NULL otherwise
 */
char *find_command_in_path(const char *command)
{
    char *path, *path_copy, *dir;
    static char full_path[1024];
    struct stat st;

    /* PATH-ı al */
    path = getenv("PATH");
    if (!path)
        return (NULL);

    /* PATH-ın surətini çıxart (strtok üçün) */
    path_copy = strdup(path);
    if (!path_copy)
        return (NULL);

    /* PATH-ı : işarəsinə görə ayır */
    dir = strtok(path_copy, ":");
    while (dir)
    {
        /* tam path düzəlt: dir + "/" + command */
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);

        /* fayl mövcuddur və icraolunandırsa qaytar */
        if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
        {
            free(path_copy);
            return (full_path);
        }

        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return (NULL);
}
