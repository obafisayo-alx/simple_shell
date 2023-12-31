#include "builtins.h"

/**
 * __exec - replace the running shell with a new program
 * @dets: arguments passed
 * Return: int
 */
int __exec(dets_t *dets)
{
	char *execute, **args = dets->tokens + 1, **env = NULL;

	if (!*args)
		return ((dets->status = EXIT_SUCCESS));

	dets->tokens = args;
	args = arrdup(args);

	if (_strchr(*args, '/') == -1)
	{
		free_list(&dets->path);
		dets->path = str_to_list(get_dict_val(dets->env, "PATH"), ':');
		execute = search_path(dets, dets->path);
	}
	else
	{
		execute = _strdup(*args);
	}
	dets->tokens -= 1;

	if (access(execute, X_OK) == 0)
	{
		env = dict_to_env(dets->env);

		free_dets(dets);
		execve(execute, args, env);
		perrorl_default(*dets->argv, dets->lineno, "Not found",
				*dets->tokens, *args, NULL);
		free(execute);
		free_tokens(&args);
		free_tokens(&env);
		exit(127);
	}
	perrorl_default(*dets->argv, dets->lineno, "Permission denied",
			*dets->tokens, *args, NULL);
	free(execute);
	free_tokens(&args);
	free_dets(dets);
	exit(126);
}
