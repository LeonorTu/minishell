/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 09:00:40 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 11:28:41 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
/*
exit at LONG MAX = -1
exit at LONG MIN = 0
*/

static void	is_overflow(t_exec *exec, char *cmd, int ato)
{
	char	*msg;

	if (ato == 0 && ft_strncmp(cmd, "0", 2) == 0)
		return ;
	else if (ato == 0 && ft_strncmp(cmd, "-9223372036854775808", 21) == 0)
		return ;
	else if (ato == -1 && ft_strncmp(cmd, "-1", 3) == 0)
		return ;
	else if (ato == -1 && ft_strncmp(cmd, "9223372036854775807", 20) == 0)
		return ;
	msg = ft_strjoin_3("jjsh: exit: ", \
		cmd, ": numeric argument required\n");
	malloc_guard(exec, msg);
	write(2, msg, ft_strlen(msg));
	free(msg);
	ft_freeall(exec);
	exec->exit_code = 255;
	exit (255);
}

int	ft_exit(t_exec *exec, char **cmd)
{
	int	tmp;

	exec->exit_code = 0;
	if (cmd[1])
	{
		tmp = ft_atoi(cmd[1]);
		if (tmp == 0 || tmp == -1)
			is_overflow(exec, cmd[1], tmp);
		if (cmd[2])
		{
			write(2, "exit\njjsh: exit: too many arguments\n", 36);
			ft_freeall_n_envp(exec);
			exec->exit_code = 1;
			return (1);
		}
		exec->exit_code = tmp;
		if (tmp < 0)
			exec->exit_code = 256 + tmp;
	}
	ft_freeall(exec);
	if (exec->cmd_count == 1)
		write(2, "exit\n", 5);
	exit (exec->exit_code);
}
