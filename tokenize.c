#include "main.h"

/**
 * tokenize - splits a string into tokens (arguments)
 * @line: input line
 * @args: array to store tokens
 * Return: number of tokens
 */
int tokenize(char *line, char **args)
{
    char *token;
    int i = 0;

    token = strtok(line, " \t\r\n");
    while (token && i < 63)
    {
        args[i++] = token;
        token = strtok(NULL, " \t\r\n");
    }
    args[i] = NULL;
    return (i);
}

