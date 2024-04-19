/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokenstrs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 14:13:00 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 10:44:10 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	parse_redir(t_exec *exec, char **dst, t_iterator *iter)
{
	ft_substr_custom(dst, exec->read_line, \
	exec->token[iter->token_iter].location.start, \
	exec->token[iter->token_iter].location.len);
	if (!dst)
		ft_error(exec, "malloc error", MALLOC_ERROR);
	iter->token_iter = iter->token_iter + 1;
	iter->redir_iter = iter->redir_iter + 1;
}

/*Malloc substring from char* start and len in t_token struct*/
void	parse_string(t_exec *exec, char **dst, t_iterator *iter)
{
	ft_substr_custom(dst, exec->read_line, \
	exec->token[iter->token_iter].location.start, \
	exec->token[iter->token_iter].location.len);
	if (!dst)
		ft_error(exec, "malloc error", MALLOC_ERROR);
	iter->token_iter = iter->token_iter + 1;
}

static bool	is_not_special_char(t_token *token)
{
	if (token->type != EOL_TOKEN
		&& token->type != SPACE_TOKEN
		&& token->type != REDIR_APPEND_TOKEN
		&& token->type != REDIR_IN_TOKEN
		&& token->type != REDIR_OUT_TOKEN
		&& token->type != REDIR_HEREDOC_TOKEN
		&& token->type != PIPE_TOKEN)
		return (true);
	return (false);
}

/*
	** Malloc substring until not special char
	** str[0] = string result
	** str[1] = parsed string
	** str[2] = temporary string to free
*/
void	parse_dquote(t_exec *exec, char **dst, t_iterator *iter, bool is_expand)
{
	size_t	quote;
	char	*str[3];

	quote = iter->token_iter;
	str[0] = ft_strdup("");
	if (!str[0])
		ft_error(exec, "malloc error", MALLOC_ERROR);
	while (is_not_special_char(&exec->token[quote]))
		quote++;
	while (iter->token_iter < quote)
	{
		str[2] = str[0];
		if (exec->token[iter->token_iter].type == DOLLAR_TOKEN)
			parse_dollar(exec, &str[1], iter, is_expand);
		else
			parse_string(exec, &str[1], iter);
		ft_strjoin_custom(&str[0], str[2], str[1]);
		if (!str[0])
			ft_error(exec, "malloc error", MALLOC_ERROR);
	}
	*dst = str[0];
}
