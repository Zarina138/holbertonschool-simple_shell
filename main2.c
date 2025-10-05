#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * main - simple UNIX command line interpreter
 *
 * Return: 0 on success
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    pid_t pid;
    char *argv[2];

    while (1)
    {
        /* Prompt yalnız interaktiv terminalda */
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "$ ", 2);

        nread = getline(&line, &len, stdin);
        if (nread == -1) /* Ctrl+D */
        {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            continue;
        }

        if (pid == 0)
        {
            /* Child process */
            argv[0] = line;
            argv[1] = NULL;
            execve(line, argv, NULL); /* NULL environment if not needed */
            perror("Error");           /* Executable not found */
            exit(EXIT_FAILURE);
        }
        else
        {
            /* Parent process */
            wait(NULL);
        }
    }

    free(line);
    return 0;
}
