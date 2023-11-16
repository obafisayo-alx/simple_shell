#include "hsh.h"

/**
 * exec - This is used to execute the commands
 * @dets: This points to the details for execution
 * 
 * Return: This returns the status
*/
int exec(dets_t *dets)
{
	if (_strchr(*dets->tokens, '/') == -1)
	{
		printf("/ detected ");
		free_list(&dets->path);
		dets->path = str_to_list(getenv("PATH")), ':');
		dets->exe = get_path(dets, dets->path);
		printf("dets->exe path found = %s", dets->exe);
	}
	else
	{
		dets->exe = _strdup(*dets->tokens);
	}
	if (dets->exe && access(dets->exe, X_OK) == 0)
	{
		return (_exec(dets));
	}
	if (dets->exe)
	{
		perrorl_default(*dets->argv, dets->linenom, "Permission denied",
				*dets->tokens, NULL);
		dets->status = 126;
	}
	else
	{
		perrorl_default(*dets->argv, dets->linenom, "not found",
				*dets->tokens, NULL);
		dets->status = 127;
	}
	return (dets->status);
}

/**
 * _exec - This is used to fork and execute the current command
 * @dets: This is the shell details
 * Return: This is the exit status of the child process
*/
int _exec(dets_t *dets)
{
	char *exec, **av, **envt;

	switch (fork())
	{
	case 0:
		exec = dets->exe;
		av = dets->tokens;
		envt = dict_to_envt(dets->envt);

		dets->exe = NULL;
		dets->tokens = NULL;
		free_dets(dets);

		execve(exec, av, envt);
		perror(*av);

		if (dets->file)
			close(dets->filenom);
		
		free(exec);
		free_string_array(av);
		free_string_array(envt);
		exit(EXIT_FAILURE);
		break;

	case -1:
		perrorl_default(*dets->argv, dets->linenom, "Cannot fork", NULL);
		dets->status = 2;
		break;

	default:
		wait(&dets->status);
		dets->status = WEXITSTATUS(dets->status);
		break;
	}
	free(dets->exe);
	dets->exe = NULL;

	return (dets->status);
}
