/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_syntax.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 11:46:38 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 10:44:10 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	is_redir(t_token *token)
{
	if (token->type == REDIR_IN_TOKEN
		|| token->type == REDIR_OUT_TOKEN
		|| token->type == REDIR_APPEND_TOKEN
		|| token->type == REDIR_HEREDOC_TOKEN)
		return (true);
	return (false);
}

static int	print_syntax_error(t_token *token)
{
	char	*to_free;

	if (token->location.start[0] == '\0')
	{
		ft_putstr_fd("jjsh-1.0$ ", 2);
		ft_putendl_fd("syntax error near unexpected token 'newline'", 2);
	}
	else
	{
		to_free = ft_substr(token->location.start, 0, token->location.len);
		if (!to_free)
			exit (2);
		ft_putstr_fd("jjsh-1.0$ ", 2);
		ft_putstr_fd("syntax error near unexpected token '", 2);
		ft_putstr_fd(to_free, 2);
		ft_putendl_fd("'", 2);
		free(to_free);
	}
	return (258);
}

static int	pipe_syntax(t_token *token, size_t i)
{
	if (token[i + 1].type == SPACE_TOKEN)
		i++;
	if (token[i + 1].type == PIPE_TOKEN
		|| token[i + 1].type == SPACE_TOKEN
		|| token[i + 1].type == EOL_TOKEN)
		return (1);
	return (0);
}

/*
	checking for unclosed quotes ok
	check for redirection file name
	pipe with no command
*/
int	check_syntax(t_token *token)
{
	size_t	i;

	i = 0;
	if (token[i].type == SPACE_TOKEN)
		i++;
	if (token[i].type == PIPE_TOKEN)
		return (print_syntax_error(&token[i]));
	while (token[i].type != EOL_TOKEN)
	{
		if (token[i].type == ERROR_TOKEN)
			return (print_syntax_error(&token[i]));
		if (is_redir(&token[i]))
		{
			if (token[i + 1].type == SPACE_TOKEN)
				i++;
			if (is_redir(&token[i + 1]) || token[i + 1].type == EOL_TOKEN
				|| token[i + 1].type == PIPE_TOKEN)
				return (print_syntax_error(&token[i + 1]));
		}
		if (token[i].type == PIPE_TOKEN)
			if (pipe_syntax(token, i))
				return (print_syntax_error(&token[i]));
		i++;
	}
	return (0);
}
