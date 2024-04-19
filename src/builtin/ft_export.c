/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 10:23:45 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 10:43:36 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	replace_env(t_exec *exec, char *exp_arg, int i, bool value)
{
	char	*env;

	if (value == false)
		return (2);
	env = exec->envp[i];
	free(env);
	exec->envp[i] = ft_strdup(exp_arg);
	malloc_guard(exec, exec->envp[i]);
	return (2);
}

static int	run_export(t_exec *exec, char *exp_arg, size_t len, bool value)
{
	char	*new;
	char	*key;
	int		e;
	int		i;

	new = ft_substr(exp_arg, 0, len);
	malloc_guard(exec, new);
	i = -1;
	while (exec->envp[++i])
	{
		e = 0;
		while (exec->envp[i][e] != '=' && exec->envp[i][e] != '\0')
			e++;
		key = ft_substr(exec->envp[i], 0, e);
		malloc_guard(exec, key);
		if (ft_strncmp(key, new, ft_strlen(key) + 1) == 0)
		{
			free(new);
			free(key);
			return (replace_env(exec, exp_arg, i, value));
		}
		free(key);
	}
	free(new);
	return (0);
}

static int	read_export_arg(t_exec *exec, char *exp_arg)
{
	size_t	c;
	bool	value;

	c = 0;
	value = false;
	while (1)
	{
		if (validate_key(exec, exp_arg, c) == 1)
			return (1);
		if (exp_arg[c] == '=')
		{
			value = true;
			break ;
		}
		c++;
		if (exp_arg[c] == '\0')
			break ;
	}
	return (run_export(exec, exp_arg, c, value));
}

static void	call_export(t_exec *exec, char *cmd)
{
	size_t	size;
	char	**env;

	size = ft_arrlen(exec->envp);
	env = ft_calloc(size + 2, sizeof(char *));
	if (!env)
		ft_error(exec, "Malloc error\n", MALLOC_ERROR);
	ft_arrcpy(&env, exec->envp);
	free(exec->envp);
	env[size] = ft_strdup(cmd);
	if (!env[size])
		ft_error(exec, "Malloc error\n", MALLOC_ERROR);
	env[size + 1] = NULL;
	exec->envp = env;
}

int	ft_export(t_exec *exec, char **cmd)
{
	int	i;
	int	ret;

	if (!cmd[1])
	{
		i = -1;
		while (exec->envp[++i])
			printf("declare -x %s\n", exec->envp[i]);
		return (0);
	}
	i = 0;
	while (cmd[++i])
	{
		ret = read_export_arg(exec, cmd[i]);
		if (ret == 1)
			return (1);
		if (ret == 2)
			continue ;
		call_export(exec, cmd[i]);
	}
	return (0);
}
