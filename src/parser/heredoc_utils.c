/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 09:35:34 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 10:44:10 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	set_start_len(t_exec *exec, t_iterator *iter)
{
	if (exec->token[iter->token_iter].type == OPEN_DQUOTE_TOKEN)
	{
		exec->token[iter->token_iter].location.start--;
		exec->token[iter->token_iter].location.len++;
	}
	if (exec->token[iter->token_iter].type == CLOSE_DQUOTE_TOKEN)
		exec->token[iter->token_iter].location.len++;
	if (exec->token[iter->token_iter].type == SQUOTE_TOKEN)
	{
		exec->token[iter->token_iter].location.start--;
		exec->token[iter->token_iter].location.len++;
		exec->token[iter->token_iter].location.len++;
	}
}

static void	parse_heredoc(t_exec *exec, char **dst, \
t_iterator *iter, bool is_expand)
{
	char	*str[3];

	str[0] = ft_strdup("");
	if (!str[0])
		ft_error(exec, "malloc error", MALLOC_ERROR);
	while (exec->token[iter->token_iter].type != EOL_TOKEN)
	{
		str[2] = str[0];
		if (exec->token[iter->token_iter].type == DOLLAR_TOKEN)
			parse_dollar(exec, &str[1], iter, is_expand);
		else
		{
			set_start_len(exec, iter);
			parse_string(exec, &str[1], iter);
		}
		ft_strjoin_custom(&str[0], str[2], str[1]);
		if (!str[0])
			ft_error(exec, "malloc error", MALLOC_ERROR);
	}
	*dst = str[0];
}

static int	handle_null_emp(t_exec *exec, int fd, char *delimiter)
{
	if (!exec->read_line)
	{
		close(fd);
		free(delimiter);
		ft_freeall(exec);
		return (1);
	}
	if (ft_strncmp(exec->read_line, delimiter, \
	ft_strlen(delimiter) + 1) == 0)
	{
		free(delimiter);
		ft_freeall(exec);
		return (1);
	}
	return (0);
}

void	write_to_heredoc(t_exec *exec, int fd, \
char *delimiter, int is_expand)
{
	t_iterator	iter;
	char		*heredoc;

	while (1)
	{
		ft_freeall_n_envp(exec);
		initialize_exec(exec);
		iter.token_iter = 0;
		exec->read_line = readline("> ");
		if (handle_null_emp(exec, fd, delimiter))
			return ;
		heredoc_tokenizer(exec);
		parse_heredoc(exec, &heredoc, &iter, is_expand);
		write(fd, heredoc, ft_strlen(heredoc));
		write(fd, "\n", 1);
		free(heredoc);
	}
}
