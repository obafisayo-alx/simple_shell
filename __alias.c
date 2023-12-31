#include "builtins.h"

/**
 * __alias_add - add an alias to a list of aliases
 * @aliases: a pointer to a list of aliases
 * @name: the name of the alias
 * @value: the value of the alias
 */
void __alias_add(alias_t **aliases, const char *name, const char *value)
{
	alias_t *alias = get_dict_node(aliases ? *aliases : NULL, name);

	if (alias)
	{
		free(alias->val);
		alias->val = _strdup(value);
	}
	else
	{
		add_dict_node_end(aliases, name, value);
	}
}


/**
 * __alias_print - print an alias
 * @alias: the alias to print
 */
void __alias_print(alias_t *alias)
{
	write(STDOUT_FILENO, alias->key, _strlen(alias->key));
	write(STDOUT_FILENO, "='", 2);
	write(STDOUT_FILENO, alias->val, _strlen(alias->val));
	write(STDOUT_FILENO, "'\n", 2);
}


/**
 * __alias - create and display aliases
 * @dets: shell dets struct
 *
 * Return: status
 */
int __alias(dets_t *dets)
{
	alias_t *ali;
	char *name, **args = dets->tokens + 1;
	ssize_t name_len;

	dets->status = EXIT_SUCCESS;
	if (*args)
	{
		do {
			name_len = _strchr(*args, '=');
			if (name_len == -1)
			{
				ali = get_dict_node(dets->aliases, *args);
				if (ali)
				{
					__alias_print(ali);
				}
				else
				{
					perrorl("not found", *dets->tokens, *args, NULL);
					dets->status = EXIT_FAILURE;
				}
			}
			else
			{
				name = _strndup(*args, name_len);
				__alias_add(&dets->aliases, name, *args + name_len + 1);
				free(name);
			}
		} while (*++args);
	}
	else
	{
		for (ali = dets->aliases; ali; ali = ali->next)
			__alias_print(ali);
	}
	return (dets->status);
}
