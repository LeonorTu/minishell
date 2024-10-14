/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 09:06:14 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 10:44:10 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	run_heredoc(t_exec *exec, int fd, char *delimiter, int is_expand)
{
	int		heredoc_child;
	int		status;

	heredoc_child = fork();
	togglesignal(IGNORE);
	if (heredoc_child == -1)
		ft_error(exec, "fork error", FORK_ERROR);
	if (heredoc_child == 0)
	{
		togglesignal(HEREDOC);
		write_to_heredoc(exec, fd, delimiter, is_expand);
		togglesignal(HANDLER);
		exit(0);
	}
	waitpid(heredoc_child, &status, 0);
	togglesignal(HANDLER);
	free(delimiter);
	if (status == SIGINT)
	{
		printf("\n");
		return (1);
	}
	return (0);
}

static void	create_heredoc_name(t_exec *exec, char **heredoc_name, int id)
{
	char	*tmp;

	tmp = ft_itoa(id);
	if (!tmp)
		ft_error(exec, "malloc error", MALLOC_ERROR);
	*heredoc_name = ft_strjoin("/tmp/heredoc_", tmp);
	if (!*heredoc_name)
	{
		free(tmp);
		ft_error(exec, "malloc error", MALLOC_ERROR);
	}
	free(tmp);
	unlink(*heredoc_name);
}

/*remember to unlink heredoc file after minishell use*/
int	heredoc(t_exec *exec, char **dst, t_iterator *iter, bool is_expand)
{
	int		fd;
	char	*heredoc_name;
	char	*delimiter;

	create_heredoc_name(exec, &heredoc_name, iter->cmd_count);
	fd = open(heredoc_name, O_TRUNC | O_CREAT | O_RDWR, 0644);
	if (fd == -1)
		ft_error(exec, "open error", MALLOC_ERROR);
	delimiter = ft_strdup(exec->cmd[iter->cmd_count].redir[iter->redir_iter]);
	if (!delimiter)
		ft_error(exec, "malloc error", MALLOC_ERROR);
	if (run_heredoc(exec, fd, delimiter, is_expand))
		return (1);
	free(*dst);
	*dst = heredoc_name;
	close(fd);
	return (0);
}
