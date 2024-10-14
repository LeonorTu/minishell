/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 14:28:26 by jtu               #+#    #+#             */
/*   Updated: 2024/04/19 10:43:36 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	close_pipes(t_exec *exec)
{
	size_t	i;

	i = 0;
	while (i < exec->cmd_count - 1)
	{
		close(exec->pipes[i][RD]);
		close(exec->pipes[i][WR]);
		i++;
	}
}

void	pipe_dup(size_t i, t_exec *exec)
{
	if (i == 0)
	{
		if (dup2(exec->pipes[i][WR], STDOUT_FILENO) < 0)
			error_exit(exec, error_msg(NULL, strerror(errno), false), 1, true);
	}
	else if (i == exec->cmd_count - 1)
	{
		if (dup2(exec->pipes[i - 1][RD], STDIN_FILENO) < 0)
			error_exit(exec, error_msg(NULL, strerror(errno), false), 1, true);
	}
	else
	{
		if (dup2(exec->pipes[i - 1][RD], STDIN_FILENO) < 0 || \
		dup2(exec->pipes[i][WR], STDOUT_FILENO) < 0)
			error_exit(exec, error_msg(NULL, strerror(errno), false), 1, true);
	}
	close_pipes(exec);
}

void	create_pipes(t_exec *exec)
{
	size_t	i;

	exec->pipes = (int **)malloc(sizeof(int *) * (exec->cmd_count - 1));
	if (!exec->pipes)
		error_exit(exec, error_msg(NULL, strerror(errno), false), 1, true);
	i = 0;
	while (i < exec->cmd_count - 1)
	{
		exec->pipes[i] = (int *)malloc(sizeof(int) * 2);
		if (!exec->pipes[i])
			error_exit(exec, error_msg(NULL, strerror(errno), false), 1, true);
		i++;
	}
	i = 0;
	while (i < exec->cmd_count - 1)
	{
		if (pipe(exec->pipes[i]) == -1)
			error_exit(exec, error_msg(NULL, strerror(errno), false), 1, true);
		i++;
	}
}
