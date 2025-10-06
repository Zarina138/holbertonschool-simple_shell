#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * main - Simple shell program
 * @argc: Argument count
 * @argv: Argument vector
 * @envp: Environment variables (unused)
 * Return: Exit status
 */
int main(int argc, char **argv, char **envp)
{
    char *line = NULL;
    size_t len = 0;
    char *args[100];
    int line_no = 0;
    int status = 0;
    char *token, *fullpath;
    pid_t pid;
    int wstatus;

    (void)argc;
    (void)envp;

    while (1)
    {
        line_no++;
        if (isatty(STDIN_FILENO))
            printf("($) ");

        if (getline(&line, &len, stdin) == -1)
        {
            if (isatty(STDIN_FILENO))
                putchar('\n');
            break;
        }

        line[strcspn(line, "\n")] = 0;

        if (line[0] == '\0')
            continue;

        /* Tokenize input */
        int i = 0;
        token = strtok(line, " \t");
        while (token && i < 99)
        {
            args[i++] = token;
            token = strtok(NULL, " \t");
        }
        args[i] = NULL;

        if (!args[0])
            continue;

        /* Built-in: exit */
        if (strcmp(args[0], "exit") == 0)
        {
            free(line);
            return status;
        }

        /* Built-in: env */
        if (strcmp(args[0], "env") == 0)
        {
            for (int j = 0; environ[j]; j++)
                printf("%s\n", environ[j]);
            continue;
        }

        /* Find command in PATH */
        fullpath = find_command(args[0]);
        if (!fullpath)
        {
            fprintf(stderr, "%s: %d: %s: not found\n", argv[0], line_no, args[0]);
            status = 127;
            continue;
        }

        /* Execute command */
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(fullpath);
            free(line);
            exit(1);
        }

        if (pid == 0)
        {
            execve(fullpath, args, environ);
            perror("execve");
            exit(1);
        }
        else
        {
            waitpid(pid, &wstatus, 0);
            if (WIFEXITED(wstatus))
                status = WEXITSTATUS(wstatus);
            else
                status = 1;
        }

        free(fullpath);
    }

    free(line);
    return status;
}
         
