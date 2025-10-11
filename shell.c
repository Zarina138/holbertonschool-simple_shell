#include "main.h"

/**
 * main - simple shell 0.2+
 * Return: 0 on success
 */
int main(void)
{
    char *line = NULL, *args[64];
    ssize_t nread;
    size_t len = 0;
    int status = 1;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "$ ", 2);

        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            free(line);
            break;
        }

        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        if (tokenize(line, args) == 0)
            continue;

        status = execute(args);
    }

    return (status);
}

