/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 14:12:01 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 11:32:19 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_realloc_array(char ***args, size_t size)
{
	char	**new;
	char	**tmp;
	size_t	i;

	tmp = *args;
	new = ft_calloc(size, sizeof(char *));
	if (!new)
	{
		ft_freearr(&tmp);
		return (MALLOC_ERROR);
	}
	i = 0;
	while (i < size - 2)
	{
		new[i] = tmp[i];
		i++;
	}
	free(tmp);
	*args = new;
	return (0);
}

int	ft_substr_custom(char **dst, char const *s, char *start, size_t len)
{
	char	*ptr;

	if (!s || !start)
		return (NULL_ERROR);
	ptr = malloc((len +1) * sizeof(char));
	if (ptr == NULL)
		return (MALLOC_ERROR);
	ft_memcpy(ptr, start, len);
	ptr[len] = 0;
	*dst = ptr;
	return (0);
}

int	ft_strjoin_custom(char **dst, char *s1, char *s2)
{
	size_t	flen;
	char	*ptr;

	if (s1 == NULL || s2 == NULL)
		return (NULL_ERROR);
	flen = ft_strlen(s1) + ft_strlen(s2);
	ptr = malloc(flen +1);
	if (!ptr)
		return (MALLOC_ERROR);
	ft_memmove(ptr, s1, ft_strlen(s1));
	ft_memmove(ptr + ft_strlen(s1), s2, ft_strlen(s2));
	ptr[flen] = 0;
	free(s1);
	free(s2);
	*dst = ptr;
	return (0);
}

void	init_data(t_exec *exec, t_iterator *iter)
{
	size_t	i;
	size_t	pipe_count;

	iter->token_iter = 0;
	iter->cmd_count = -1;
	i = 0;
	pipe_count = 0;
	while (exec->token[i].type != EOL_TOKEN)
	{
		if (exec->token[i].type == PIPE_TOKEN)
			pipe_count++;
		i++;
	}
	exec->cmd_count = pipe_count + 1;
	exec->cmd = ft_calloc(pipe_count + 2, sizeof(t_cmd));
	if (!exec->cmd)
		ft_error(exec, "malloc error", MALLOC_ERROR);
}
