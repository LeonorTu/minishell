/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 09:02:45 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 11:35:43 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "char_iter.h"

# include <stdlib.h>
# include <string.h>
# include <stdio.h>

# define DELIMITER "|<>\"\'$ '\t'"

typedef enum e_token_type
{
	PIPE_TOKEN,
	REDIR_IN_TOKEN,
	REDIR_OUT_TOKEN,
	REDIR_APPEND_TOKEN,
	REDIR_HEREDOC_TOKEN,
	OPEN_DQUOTE_TOKEN,
	CLOSE_DQUOTE_TOKEN,
	SQUOTE_TOKEN,
	STRING_TOKEN,
	DOLLAR_TOKEN,
	SPACE_TOKEN,
	EOL_TOKEN,
	ERROR_TOKEN,
}	t_token_type;

typedef struct s_slice
{
	char	*start;
	size_t	len;
}	t_slice;

typedef struct s_token
{
	t_token_type	type;
	t_slice			location;
}	t_token;

/* t_tokens */
void	take_pipe(t_char_iter *iter, t_token *token);
void	take_redir_in(t_char_iter *iter, t_token *token);
void	take_redir_out(t_char_iter *iter, t_token *token);
void	take_dquote(t_char_iter *iter, t_token *token, int *d_flag);
void	take_squote(t_char_iter *iter, t_token *token);
void	take_string(t_char_iter *iter, t_token *token, int *d_flag);
void	take_dollar(t_char_iter *iter, t_token *token, int *d_flag);
void	take_space(t_char_iter *iter, t_token *token);
void	take_eol(t_char_iter *iter, t_token *token);
void	take_error(t_char_iter *iter, t_token *token, int d_flag);
void	take_exp(t_char_iter *iter, t_token *token);

/*utils*/
int		check_syntax(t_token *token);
int		ft_realloc(t_token **token, size_t size);
#endif