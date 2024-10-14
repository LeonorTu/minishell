/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_tokenizer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 09:35:34 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 10:44:10 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	heredoc_take_just_dollar(t_char_iter *iter, t_token *token)
{
	token->type = STRING_TOKEN;
	token->location.start = char_iter_cursor(iter);
	token->location.len = 1;
	char_iter_next(iter);
	if (char_iter_peek(iter) == '?')
	{
		token->type = DOLLAR_TOKEN;
		token->location.start = char_iter_cursor(iter);
		char_iter_next(iter);
	}
}

static void	heredoc_take_dollar(t_char_iter *iter, t_token *token)
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
}

static void	heredoc_take_string(t_char_iter *iter, t_token *token)
{
	token->location.start = char_iter_cursor(iter);
	token->location.len = 0;
	token->type = STRING_TOKEN;
	while (char_iter_cursor(iter) != iter->end
		&& char_iter_peek(iter) != '$')
	{
		char_iter_next(iter);
		token->location.len++;
	}
}

static void	heredoc_tokenizer_loop(t_token *token, t_char_iter *iter)
{
	if (iter->start[0] == '$'
		&& (iter->start[1] == '\0' || iter->start[1] == '?'
			|| iter->start[1] == '=' || ft_isdigit(iter->start[1])
			|| iter->start[1] == '$' || iter->start[1] == ' '))
		heredoc_take_just_dollar(iter, token);
	else if (iter->start[0] == '$' && iter->start[1] != '?'
		&& iter->start[1] != '\0')
		heredoc_take_dollar(iter, token);
	else
		heredoc_take_string(iter, token);
}

void	heredoc_tokenizer(t_exec *exec)
{
	t_char_iter		iter;
	t_token			*token;
	size_t			i;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		ft_error(exec, "Malloc error\n", MALLOC_ERROR);
	iter = char_iter_constructor(exec->read_line, ft_strlen(exec->read_line));
	i = 0;
	while (char_iter_cursor(&iter) != iter.end)
	{
		heredoc_tokenizer_loop(&token[i], &iter);
		i++;
		if (ft_realloc(&token, i + 1))
			ft_error(exec, "Malloc error\n", MALLOC_ERROR);
	}
	take_eol(&iter, &token[i]);
	exec->token = token;
}
