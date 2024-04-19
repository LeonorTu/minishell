/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokendollar.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 11:51:16 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 10:44:10 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*ft_getenv_loop(t_exec *exec, char *key, size_t i)
{
	char	*value;

	if (ft_strncmp(exec->envp[i], key, ft_strlen(key)) == 0)
	{
		value = ft_strdup(exec->envp[i] + ft_strlen(key));
		if (!value)
		{
			free(key);
			ft_error(exec, "malloc error", MALLOC_ERROR);
		}
		free(key);
		return (value);
	}
	return (NULL);
}

char	*ft_getenv(t_exec *exec, char *key)
{
	size_t	i;
	char	*tmp;
	char	*value;

	tmp = key;
	key = ft_strjoin(tmp, "=");
	malloc_guard(exec, key);
	free(tmp);
	i = -1;
	while (exec->envp[++i])
	{
		value = ft_getenv_loop(exec, key, i);
		if (value)
			return (value);
	}
	free(key);
	return (NULL);
}

static void	not_exp(t_exec *exec, char **dst, t_iterator *iter)
{
	exec->token[iter->token_iter].location.start--;
	exec->token[iter->token_iter].location.len++;
	parse_string(exec, dst, iter);
}

/*Malloc substring and  expand $ variable*/
void	parse_dollar(t_exec *exec, char **dst, t_iterator *iter, bool is_expand)
{
	char	*env_str;

	if (is_expand == false)
		return (not_exp(exec, dst, iter));
	ft_substr_custom(&env_str, exec->read_line, \
	exec->token[iter->token_iter].location.start, \
	exec->token[iter->token_iter].location.len);
	if (!env_str)
		ft_error(exec, "malloc error", MALLOC_ERROR);
	if (ft_strncmp(env_str, "?", 2) == 0)
	{
		*dst = ft_itoa(exec->exit_code);
		free(env_str);
	}
	else
	{
		*dst = ft_getenv(exec, env_str);
		if (!*dst)
			*dst = ft_strdup("");
	}
	iter->token_iter = iter->token_iter + 1;
}
