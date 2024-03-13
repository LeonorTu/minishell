/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jberay <jberay@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 10:23:45 by jberay            #+#    #+#             */
/*   Updated: 2024/03/13 13:46:50 by jberay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin(t_exec *exec)
{
	if (ft_strncmp(exec->cmd->cmd[0], "export", 7) == 0)
		ft_export(exec);
	// else if (ft_strncmp(cmd->cmd[0], "echo", 5) == 0)
	// 	ft_echo(cmd);
	// else if (ft_strncmp(cmd->cmd[0], "cd", 3) == 0)
	// 	ft_cd(cmd);
	// else if (ft_strncmp(cmd->cmd[0], "pwd", 4) == 0)
	// 	ft_pwd(cmd);
	// else if (ft_strncmp(cmd->cmd[0], "unset", 6) == 0)
	// 	ft_unset(cmd);
	// else if (ft_strncmp(cmd->cmd[0], "env", 4) == 0)
	// 	ft_env(cmd);
	// else if (ft_strncmp(cmd->cmd[0], "exit", 5) == 0)
	// 	ft_exit(cmd);
}

void	ft_export(t_exec *exec)
{
	size_t	c;
	size_t	size;
	char	**env;
	char	*exp_arg;

	c = 0;
	exp_arg = exec->cmd[0].cmd[1];
	while (exp_arg[c])
	{
		if (ft_isalnum(exp_arg[c]) == 0 && exp_arg[c] != '_' && exp_arg[c] != '=')
		{
			ft_putstr_fd("jjsh-1.0$ export: `", 2);
			ft_putstr_fd(exp_arg, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			return ;
		}
		c++;
	}
	size = ft_arrlen(exec->envp);
	env = ft_calloc(size + 1, sizeof(char *));
	if (!env)
		exit(2);
	env = exec->envp;
	ft_strchr(exec->cmd[0].cmd[1], '=');
	env[size] = (exec->cmd[0].cmd[1]);
	env[size + 1] = 0;
	exec->envp = env;
}
