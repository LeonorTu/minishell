/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 12:31:06 by jtu               #+#    #+#             */
/*   Updated: 2024/04/19 10:43:36 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	handle_outfile(t_exec *exec, char *redir, char *file, bool quit)
{
	int	fd_out;

	if (!ft_strncmp(redir, ">>", 3))
		fd_out = open(file, O_CREAT | O_RDWR | O_APPEND, 0644);
	else
		fd_out = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd_out < 0)
		return (error_exit(exec, error_msg(file, \
			strerror(errno), false), 1, quit));
	if (dup2(fd_out, STDOUT_FILENO) < 0)
		return (error_exit(exec, error_msg(NULL, \
			strerror(errno), false), 1, quit));
	close(fd_out);
	return (0);
}

static int	handle_infile(t_exec *exec, char *file, bool quit)
{
	int	fd_in;

	fd_in = open(file, O_RDONLY);
	if (fd_in < 0)
		return (error_exit(exec, error_msg(file, \
			strerror(errno), false), 1, quit));
	if (dup2(fd_in, STDIN_FILENO) < 0)
		return (error_exit(exec, error_msg(NULL, \
			strerror(errno), false), 1, quit));
	close(fd_in);
	return (0);
}

int	check_redirections(t_exec *exec, t_cmd parsed_cmd, bool quit)
{
	int	i;

	i = 0;
	while (parsed_cmd.redir[i] != NULL)
	{
		if (!ft_strncmp(parsed_cmd.redir[i], "<", 2) || \
			!ft_strncmp(parsed_cmd.redir[i], "<<", 3))
		{
			if (handle_infile(exec, parsed_cmd.redir[i + 1], quit) == 1)
				return (1);
		}
		if (!ft_strncmp(parsed_cmd.redir[i], ">", 2) || \
			!ft_strncmp(parsed_cmd.redir[i], ">>", 3))
		{
			if (handle_outfile(exec, parsed_cmd.redir[i], \
			parsed_cmd.redir[i + 1], quit) == 1)
				return (1);
		}
		i = i + 2;
	}
	return (0);
}
