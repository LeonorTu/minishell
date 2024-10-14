/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_strs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 09:48:23 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 11:34:11 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	take_dollar(t_char_iter *iter, t_token *token, int *d_flag)
{
	token->type = DOLLAR_TOKEN;
	token->location.start = char_iter_cursor(iter) + 1;
	token->location.len = 0;
	char_iter_next(iter);
	while (char_iter_cursor(iter) != iter->end
		&& !ft_strchr(DELIMITER, char_iter_peek(iter))
		&& char_iter_peek(iter) != '/'
		&& char_iter_peek(iter) != ':'
		&& char_iter_peek(iter) != '@'
		&& char_iter_peek(iter) != '=')
	{
		char_iter_next(iter);
		token->location.len++;
	}
	if (char_iter_cursor(iter) == iter->end && *d_flag % 2 == 1)
	{
		take_error(iter, token, *d_flag);
		return ;
	}
}

void	take_space(t_char_iter *iter, t_token *token)
{
	token->type = SPACE_TOKEN;
	token->location.start = char_iter_cursor(iter);
	token->location.len = 0;
	while (char_iter_cursor(iter) != iter->end
		&& (char_iter_peek(iter) == ' '
			|| char_iter_peek(iter) == '\t'))
	{
		char_iter_next(iter);
		token->location.len++;
	}
}

void	take_string(t_char_iter *iter, t_token *token, int *d_flag)
{
	token->location.start = char_iter_cursor(iter);
	token->location.len = 0;
	token->type = STRING_TOKEN;
	if (*d_flag % 2 == 0)
	{
		while (char_iter_cursor(iter) != iter->end
			&& !ft_strchr(DELIMITER, char_iter_peek(iter)))
		{
			char_iter_next(iter);
			token->location.len++;
		}
	}
	else
	{
		while (char_iter_cursor(iter) != iter->end
			&& char_iter_peek(iter) != '"'
			&& char_iter_peek(iter) != '$')
		{
			char_iter_next(iter);
			token->location.len++;
		}
		if (char_iter_cursor(iter) == iter->end)
			take_error(iter, token, *d_flag);
	}
}

void	take_dquote(t_char_iter *iter, t_token *token, int *d_flag)
{
	token->location.len = 0;
	token->location.start = char_iter_cursor(iter);
	if (*d_flag % 2 == 0)
	{
		*d_flag += 1;
		token->location.start++;
		token->type = OPEN_DQUOTE_TOKEN;
		char_iter_next(iter);
		while (char_iter_cursor(iter) != iter->end
			&& char_iter_peek(iter) != '"')
		{
			if (char_iter_peek(iter) == '$'
				&& char_iter_peek_next(iter) != ' '
				&& char_iter_peek_next(iter) != '"')
				break ;
			char_iter_next(iter);
			token->location.len++;
		}
		if (char_iter_cursor(iter) == iter->end)
			take_error(iter, token, *d_flag);
		return ;
	}
	token->type = CLOSE_DQUOTE_TOKEN;
	*d_flag += 1;
	char_iter_next(iter);
}

void	take_squote(t_char_iter *iter, t_token *token)
{
	token->type = SQUOTE_TOKEN;
	token->location.start = char_iter_cursor(iter) + 1;
	token->location.len = 0;
	char_iter_next(iter);
	while (char_iter_cursor(iter) != iter->end
		&& char_iter_peek(iter) != '\'')
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
