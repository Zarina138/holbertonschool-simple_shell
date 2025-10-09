#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ;

/* prompt.c */
void display_prompt(void);
char *read_command(void);

/* path.c */
char *get_env_value(const char *name);
char *find_command_in_path(const char *command);

/* execute.c */
int execute_command(char **args, char *shell_name);

/* utils.c */
char **split_line(char *line);
void free_args(char **args);

#endif
