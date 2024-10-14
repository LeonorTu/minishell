/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 14:11:18 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 10:44:10 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	take_eol(t_char_iter *iter, t_token *token)
{
	token->type = EOL_TOKEN;
	token->location.start = char_iter_cursor(iter);
	token->location.len = 0;
}

void	take_pipe(t_char_iter *iter, t_token *token)
{
	token->type = PIPE_TOKEN;
	token->location.start = char_iter_cursor(iter);
	token->location.len = 1;
	char_iter_next(iter);
}

char	*char_find_dq(t_char_iter *self)
{
	size_t	len;

	len = 1;
	while (*(self->end - len) != '"')
	{
		len++;
	}
	return (self->end - len);
}

/*error token representing syntax error*/
void	take_error(t_char_iter *iter, t_token *token, int d_flag)
{
	token->type = ERROR_TOKEN;
	token->location.start = char_iter_cursor(iter);
	if (d_flag % 2 == 1)
	{
		token->location.start = char_find_dq(iter);
		*iter = char_iter_constructor(token->location.start, \
		ft_strlen(token->location.start));
		token->location.len = 1;
		return ;
	}
	printf("error token %s\n", token->location.start);
	token->location.len = 0;
	while (char_iter_cursor(iter) != iter->end)
	{
		char_iter_next(iter);
		token->location.len++;
	}
}

/*Re allocation of token array*/
int	ft_realloc(t_token **token, size_t size)
{
	t_token		*new;
	size_t		i;

	new = ft_calloc(size, sizeof(t_token));
	if (!new)
		return (MALLOC_ERROR);
	i = 0;
	while (i < size - 1)
	{
		new[i] = (*token)[i];
		i++;
	}
	free(*token);
	*token = new;
	return (0);
}
