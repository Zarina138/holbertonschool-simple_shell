#include "main.h"

/**
 * execute - executes a command with PATH handling
 * @args: command and arguments
 * Return: 0 on success, 1 otherwise
 */
int execute(char **args)
{
    char *cmd_path;
    pid_t pid;
    int status;

    if (args[0] == NULL)
        return (1);

    cmd_path = find_in_path(args[0]);

    if (!cmd_path)
    {
        fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
        return (1);
    }

    pid = fork();
    if (pid == 0)
    {
        if (execve(cmd_path, args, environ) == -1)
            perror("execve");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
        perror("fork");
    else
        waitpid(pid, &status, 0);

    if (cmd_path != args[0])
        free(cmd_path);

    return (1);
}
