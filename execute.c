#include "shell.h"

/**
 * execute_command - Əmri icra edir (PATH daxil olmaqla)
 * @args: Əmr və arqumentlər massividir
 * Return: Əmr uğurla icra olunarsa 0, tapılmazsa -1
 */
int execute_command(char **args)
{
    pid_t pid;
    int status;
    char *cmd_path;

    if (args == NULL || args[0] == NULL)
        return (-1);

   
    cmd_path = find_command_in_path(args[0]);
    if (cmd_path == NULL)
    {
        
        fprintf(stderr, "%s: command not found\n", args[0]);
        return (-1);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(cmd_path);
        return (-1);
    }

    if (pid == 0)
    {
        
        if (execve(cmd_path, args, environ) == -1)
        {
            perror("execve");
            free(cmd_path);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        
        waitpid(pid, &status, 0);
    }

    free(cmd_path);
    return (0);
}
