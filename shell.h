#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

extern char **environ;

int prompt_loop(void);
char **split_line(char *line);
int execute_command(char **args);
char *find_command_in_path(char *cmd);

#endif /* SHELL_H */

