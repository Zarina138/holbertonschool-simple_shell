#include "shell.h"

/**
 * execute_command - Runs a command entered by the user
 * @args: Command and its arguments
 * @shell_name: Name of the shell (argv[0])
 * Return: 0 if "exit", 1 otherwise
 */
int execute_command(char **args, char *shell_name)
{
    pid_t pid;
    int status;
    char *cmd_path;

    if (!args || !args[0])
        return (1);

    /* Built-in command: exit */
    if (strcmp(args[0], "exit") == 0)
        return (0);

    /* Search in PATH */
    cmd_path = find_command_in_path(args[0]);
    if (!cmd_path)
    {
        /* ✅ Əmr tapılmadıqda xəta mesajı */
        dprintf(STDERR_FILENO, "%s: 1: %s: not found\n", shell_name, args[0]);
        return (1);
    }

    /* Create child process */
    pid = fork();
    if (pid == 0)
    {
        if (execve(cmd_path, args, environ) == -1)
        {
            perror(shell_name);
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        perror("fork");
    }
    else
    {
        waitpid(pid, &status, 0);
    }

    free(cmd_path);
    return (1);
}

/**
 * find_command_in_path - Finds command full path in PATH
 * @command: Command name
 * Return: Full path string or NULL if not found
 */
char *find_command_in_path(char *command)
{
    char *path_env, *path_copy, *token, full_path[1024];
    struct stat st;

    if (strchr(command, '/'))
        return (access(command, X_OK) == 0 ? strdup(command) : NULL);

    path_env = getenv("PATH");
    if (!path_env)
        return (NULL);

    path_copy = strdup(path_env);
    token = strtok(path_copy, ":");

    while (token)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", token, command);
        if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
        {
            free(path_copy);
            return (strdup(full_path));
        }
        token = strtok(NULL, ":");
    }

    free(path_copy);
    return (NULL);
}
