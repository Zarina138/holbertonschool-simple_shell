#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
extern char **environ;

char *find_command_in_path(const char *command);
int execute_command(char **args, char *shell_name);

char **parse_input(char *input);
void free_args(char **args);
void display_prompt(void);
char *read_command(void);
char **split_line(char *line);

#endif
