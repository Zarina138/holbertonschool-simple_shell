#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char *prompt_and_read(void);
char **split_line(char *line);
char *find_command(char *cmd);
int execute_cmd(char **argv_exec);


char *_strdup(const char *s);

#endif

