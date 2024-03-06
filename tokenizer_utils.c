/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jberay <jberay@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 09:46:54 by jberay            #+#    #+#             */
/*   Updated: 2024/03/06 10:06:15 by jberay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	take_redir_append(t_char_iter *iter, t_token *token)
{
	token->type = REDIR_APPEND_TOKEN;
	token->location.start = char_iter_cursor(iter);
	token->location.len = 2;
	char_iter_next(iter);
	char_iter_next(iter);
}

void	take_redir_heredoc(t_char_iter *iter, t_token *token)
{
	token->type = REDIR_HEREDOC_TOKEN;
	token->location.start = char_iter_cursor(iter);
	token->location.len = 2;
	char_iter_next(iter);
	char_iter_next(iter);
}

void	take_redir_in(t_char_iter *iter, t_token *token)
{
	char	peek;

	if (char_iter_has_next(iter))
	{
		peek = char_iter_peek(iter + 1);
		if (peek == '<')
		{
			take_redir_heredoc(iter, token);
			return ;
		}
	}
	token->type = REDIR_IN_TOKEN;
	token->location.start = char_iter_cursor(iter);
	token->location.len = 1;
	char_iter_next(iter);
}

void	take_redir_out(t_char_iter *iter, t_token *token)
{
	char	peek;

	if (char_iter_has_next(iter))
	{
		peek = char_iter_peek(iter + 1);
		if (peek == '>')
		{
			take_redir_append(iter, token);
			return ;
		}
	}
	token->type = REDIR_OUT_TOKEN;
	token->location.start = char_iter_cursor(iter);
	token->location.len = 1;
	char_iter_next(iter);
}

void	take_eol(t_char_iter *iter, t_token *token)
{
	token->type = EOL_TOKEN;
	token->location.start = char_iter_cursor(iter);
	token->location.len = 0;
}