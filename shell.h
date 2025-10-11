#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;

/* Functions */
char *get_env_value(const char *name);
char *find_command_in_path(char *command);
int execute_command(char **args, char *shell_name);
char *read_line(void);
char **split_line(char *line);

#endif
