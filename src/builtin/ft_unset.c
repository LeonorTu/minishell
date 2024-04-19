/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 17:14:56 by jtu               #+#    #+#             */
/*   Updated: 2024/04/19 10:43:36 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_in_arr(char **envp, char *var)
{
	int		i;
	char	*var_equal;

	i = 0;
	var_equal = ft_strjoin(var, "=");
	while (envp[i])
	{
		if (ft_strnstr(envp[i], var_equal, ft_strlen(var_equal) + 1))
		{
			free(var_equal);
			return (i);
		}
		if (ft_strnstr(envp[i], var, ft_strlen(var) + 1)
			&& ft_strlen(envp[i]) == ft_strlen(var))
		{
			free(var_equal);
			return (i);
		}
		i++;
	}
	free(var_equal);
	return (-1);
}

int	check_invalid(char *cmd)
{
	int	i;

	i = 0;
	if ((ft_isalnum(cmd[i]) == 0 && cmd[i] != '_' \
		&& cmd[i] != '=') || ft_isdigit(cmd[0])
		|| (cmd[0] == '='))
	{
		ft_putstr_fd("jjsh: unset: `", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	return (0);
}

void	env_rpc(t_exec *exec, int len, int j, int rm)
{
	free(exec->envp[j]);
	while (j < len)
	{
		exec->envp[j] = exec->envp[j + 1];
		j++;
	}
	exec->envp[len - 1 - rm] = NULL;
}

int	ft_unset(t_exec *exec, char **cmd)
{
	int	i;
	int	j;
	int	rm;
	int	len;

	i = 0;
	j = -1;
	rm = 0;
	len = 0;
	while (exec->envp[len] != NULL)
		len++;
	if (!cmd[i])
		return (0);
	while (cmd[++i])
	{
		if (check_invalid(cmd[i]))
			return (1);
		j = is_in_arr(exec->envp, cmd[i]);
		if (j >= 0)
		{
			env_rpc(exec, len, j, rm);
			rm++;
		}
	}
	return (0);
}
