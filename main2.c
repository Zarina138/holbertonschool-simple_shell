#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * main - Simple UNIX command line interpreter
 *
 * Return: Always 0 (Success)
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
        /* Promptu yalnız interaktiv terminalda göstər */
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "$ ", 2);

        nread = getline(&line, &len, stdin);

        if (nread == -1)
        {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        pid = fork();
        if (pid == 0)
        {
            argv[0] = line;
            argv[1] = NULL;
            execve(line, argv, NULL);
            perror("Error");
            exit(1);
        }
        else
            wait(NULL);
    }

    free(line);
    return 0;
}
