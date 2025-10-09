#include "shell.h"

/**
 * get_env_value - returns the value of an environment variable
 */
char *get_env_value(const char *name)
{
	int i;
	size_t len;

	if (!name || !environ)
		return (NULL);
	len = strlen(name);

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
			return (environ[i] + len + 1);
	}
	return (NULL);
}
