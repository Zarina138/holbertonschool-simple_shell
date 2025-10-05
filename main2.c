#include "shell.h"
if (cmd == NULL)
return (-1);


argv_exec[0] = cmd;
argv_exec[1] = NULL;


pid = fork();
if (pid == -1)
{
perror("fork");
return (-1);
}


if (pid == 0)
{
/* child */
if (execve(cmd, argv_exec, environ) == -1)
{
/* execve failed */
dprintf(STDERR_FILENO, "simple_shell: %s: %s\n", cmd, strerror(errno));
_exit(EXIT_FAILURE);
}
}
else
{
/* parent */
if (waitpid(pid, &status, 0) == -1)
{
perror("waitpid");
return (-1);
}
}


return (0);
}


/**
* main - entry point for the simple shell
* Return: EXIT_SUCCESS or EXIT_FAILURE
*/
int main(void)
{
char *line = NULL;
char *cmd = NULL;


while (1)
{
line = prompt_and_read();
if (line == NULL)
{
/* EOF or read error: exit cleanly */
if (isatty(STDIN_FILENO))
write(STDOUT_FILENO, "\n", 1);
break;
}


cmd = parse_line(line);


if (cmd == NULL)
{
/* empty line or multi-word (not supported) */
free(line);
continue;
}


if (execute_cmd(cmd) == -1)
{
/* error printed by execute_cmd */
}


free(line);
}


return (EXIT_SUCCESS);
}
