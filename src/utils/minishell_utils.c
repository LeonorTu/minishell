/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 08:54:07 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 11:38:33 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	malloc_guard(t_exec *exec, void *ptr)
{
	if (!ptr)
		ft_error(exec, "Malloc error\n", MALLOC_ERROR);
}

int	expander(t_exec *exec)
{
	t_iterator	iter;
	char		*expanded;

	iter.token_iter = 0;
	iter.cmds_iter = 0;
	tokenizer(exec);
	if (check_syntax(exec->token))
		return (SYNTAX_ERROR);
	parse_exp_token(exec, &expanded, &iter, true);
	free(exec->token);
	exec->token = NULL;
	free(exec->read_line);
	exec->read_line = expanded;
	tokenizer(exec);
	if (check_syntax(exec->token))
		return (SYNTAX_ERROR);
	return (0);
}
