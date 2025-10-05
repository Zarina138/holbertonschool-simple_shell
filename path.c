#include "shell.h"

/**
 * find_command - search for a command in PATH directories
 * @cmd: command name (e.g., "ls")
 *
 * Return: malloc'd full path if found, or NULL if not found
 */
char *find_command(char *cmd)
{
    char *path_env, *path_copy, *dir;
    char full_path[1024];
    char *result = NULL;

    /* Əgər komanda absolute və ya relative yoldursa (məs: ./prog və ya /bin/ls) */
    if (access(cmd, X_OK) == 0)
        return (strdup(cmd));

    /* PATH dəyişənini al */
    path_env = getenv("PATH");
    if (!path_env)
        return (NULL);

    /* Kopya yarad (çünki strtok orijinal sətri dəyişir) */
    path_copy = strdup(path_env);
    if (!path_copy)
        return (NULL);

    dir = strtok(path_copy, ":");
    while (dir)
    {
        /* Tam yol qur: dir + "/" + cmd */
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);

        /* Əgər fayl mövcuddursa və icra edilə bilirsə */
        if (access(full_path, X_OK) == 0)
        {
            result = strdup(full_path); /* malloc-lanmış nüsxə */
            break;
        }

        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return (result); /* Tapılmadısa NULL olacaq */
}

