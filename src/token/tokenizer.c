/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 14:10:55 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 11:34:13 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	take_just_dollar(t_char_iter *iter, t_token *token)
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

void	tokenizer_loop(t_token *token, t_char_iter *iter, int *d_flag)
{
	if (iter->start[0] == '<' && *d_flag % 2 == 0)
		take_redir_in(iter, token);
	else if (iter->start[0] == '>' && *d_flag % 2 == 0)
		take_redir_out(iter, token);
	else if (iter->start[0] == '\'' && *d_flag % 2 == 0)
		take_squote(iter, token);
	else if ((iter->start[0] == ' ' || iter->start[0] == '\t')
		&& *d_flag % 2 == 0)
		take_space(iter, token);
	else if (iter->start[0] == '"')
		take_dquote(iter, token, d_flag);
	else if (iter->start[0] == 31 && *d_flag % 2 == 0)
		take_exp(iter, token);
	else if (iter->start[0] == '$'
		&& (iter->start[1] == '\0' || iter->start[1] == '?'
			|| iter->start[1] == '=' || ft_isdigit(iter->start[1])
			|| iter->start[1] == '$' || iter->start[1] == ' '))
		take_just_dollar(iter, token);
	else if (iter->start[0] == '$' && iter->start[1] != '?'
		&& iter->start[1] != '\0')
		take_dollar(iter, token, d_flag);
	else if (iter->start[0] == '|' && *d_flag % 2 == 0)
		take_pipe(iter, token);
	else
		take_string(iter, token, d_flag);
}

/*
	** This function takes a string and converts it to array of tokens
*/
void	tokenizer(t_exec *exec)
{
	t_char_iter		iter;
	t_token			*token;
	int				d_flag;
	size_t			i;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		ft_error(exec, "Malloc error\n", MALLOC_ERROR);
	iter = char_iter_constructor(exec->read_line, ft_strlen(exec->read_line));
	i = 0;
	d_flag = 0;
	while (char_iter_cursor(&iter) != iter.end)
	{
		tokenizer_loop(&token[i], &iter, &d_flag);
		i++;
		if (ft_realloc(&token, i + 1))
			ft_error(exec, "Malloc error\n", MALLOC_ERROR);
	}
	take_eol(&iter, &token[i]);
	exec->token = token;
}
