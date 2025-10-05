#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Prototypes */
char *prompt_and_read(void);
char **split_line(char *line);
int execute_cmd(char **argv_exec);
char *find_command(char *cmd, char **envp);
extern char **environ;

#endif

