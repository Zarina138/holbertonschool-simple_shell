#include "shell.h"

/**
 * split_line - splits a line into words
 */
char **split_line(char *line)
{
	char **args = NULL, *token;
	size_t bufsize = 64, i = 0;

	args = malloc(sizeof(char *) * bufsize);
	if (!args)
		return (NULL);

	token = strtok(line, " ");
	while (token)
	{
		args[i++] = strdup(token);
		token = strtok(NULL, " ");
	}
	args[i] = NULL;
	return (args);
}

/**
 * free_args - frees a list of args
 */
void free_args(char **args)
{
	size_t i = 0;

	if (!args)
		return;
	while (args[i])
		free(args[i++]);
	free(args);
}

