/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 14:11:41 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 11:34:07 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	char	peek_next;

	peek_next = *(iter->start + 1);
	if (peek_next != *iter->end)
	{
		if (peek_next == '<')
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
	char	peek_next;

	peek_next = *(iter->start + 1);
	if (peek_next != *iter->end)
	{
		if (peek_next == '>')
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

void	take_exp(t_char_iter *iter, t_token *token)
{
	token->type = SQUOTE_TOKEN;
	token->location.start = char_iter_cursor(iter) + 1;
	token->location.len = 0;
	char_iter_next(iter);
	while (char_iter_cursor(iter) != iter->end
		&& char_iter_peek(iter) != 31)
	{
		char_iter_next(iter);
		token->location.len++;
	}
	if (char_iter_cursor(iter) == iter->end)
	{
		token->type = ERROR_TOKEN;
		token->location.start--;
		token->location.len = 1;
		return ;
	}
	else
		char_iter_next(iter);
}
