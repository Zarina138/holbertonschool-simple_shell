#include "shell.h"

extern char **environ;

char *prompt_and_read(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    if (isatty(STDIN_FILENO))
        write(STDOUT_FILENO, "$ ", 2);

    nread = getline(&line, &len, stdin);
    if (nread == -1)
    {
        free(line);
        return NULL;
    }
    return line;
}

char **split_line(char *line)
{
    char **argv = NULL;
    char *token;
    size_t bufsize = 64, i = 0;

    argv = malloc(sizeof(char *) * bufsize);
    if (!argv)
        return NULL;

    token = strtok(line, " \t\r\n");
    while (token)
    {
        argv[i++] = token;
        if (i >= bufsize)
            break;
        token = strtok(NULL, " \t\r\n");
    }
    argv[i] = NULL;
    return argv;
}

int execute_cmd(char **argv_exec)
{
    pid_t pid;
    int status;
    char *cmd_path;
    char err_buf[1024];

    if (!argv_exec || !argv_exec[0])
        return -1;

    cmd_path = find_command(argv_exec[0], environ);
    if (!cmd_path)
    {
        int len = snprintf(err_buf, sizeof(err_buf), "%s: not found\n", argv_exec[0]);
        write(STDERR_FILENO, err_buf, len);
        return 127;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(cmd_path);
        return -1;
    }

    if (pid == 0)
    {
        execve(cmd_path, argv_exec, environ);
        perror(argv_exec[0]);
        free(cmd_path);
        _exit(EXIT_FAILURE);
    }
    else
        waitpid(pid, &status, 0);

    free(cmd_path);
    return 0;
}

int main(void)
{
    char *line;
    char **argv;

    while (1)
    {
        line = prompt_and_read();
        if (!line)
        {
            if (isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        argv = split_line(line);
        if (argv && argv[0])
            execute_cmd(argv);

        free(argv);
        free(line);
    }

    return EXIT_SUCCESS;
}

