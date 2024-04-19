/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_history.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 09:21:48 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 10:44:10 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	make_history(t_exec *exec)
{
	char	*history;
	char	*tmp;

	exec->history_fd = open(".history.txt", O_CREAT | O_RDWR, 0644);
	if (exec->history_fd == -1)
		ft_error(exec, "history file error", MALLOC_ERROR);
	history = get_next_line(exec->history_fd);
	while (history)
	{
		tmp = ft_strtrim(history, "\n");
		if (!tmp)
			ft_error(exec, "malloc error", MALLOC_ERROR);
		add_history(tmp);
		free(history);
		free(tmp);
		history = get_next_line(exec->history_fd);
	}
}

int	ft_addhistory(t_exec *exec)
{
	char	*tmp;

	tmp = exec->read_line;
	write(exec->history_fd, exec->read_line, \
	ft_strlen(exec->read_line));
	write(exec->history_fd, "\n", 1);
	add_history(exec->read_line);
	exec->read_line = ft_strtrim(tmp, " ");
	if (!exec->read_line)
		ft_error(exec, "malloc error", MALLOC_ERROR);
	free(tmp);
	if (!*exec->read_line)
		return (1);
	return (0);
}
