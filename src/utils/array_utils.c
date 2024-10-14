/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 08:32:34 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 10:44:10 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

size_t	ft_arrlen(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i] != NULL)
		i++;
	return (i);
}

int	ft_arrdup(char ***dst_add, char **src)
{
	size_t	src_size;
	size_t	i;
	char	**dst;

	if (!src)
		return (NULL_ERROR);
	src_size = ft_arrlen(src);
	dst = ft_calloc(src_size + 1, sizeof(char *));
	if (!dst)
		return (MALLOC_ERROR);
	i = 0;
	while (i < src_size)
	{
		dst[i] = ft_strdup(src[i]);
		if (!dst[i])
		{
			ft_freearr(&dst);
			return (MALLOC_ERROR);
		}
		i++;
	}
	dst[i] = NULL;
	*dst_add = dst;
	return (NO_ERR);
}

void	ft_arrcpy(char ***dst_add, char **src)
{
	size_t	src_size;
	size_t	i;
	char	**dst;

	dst = *dst_add;
	src_size = ft_arrlen(src);
	i = 0;
	while (i < src_size)
	{
		dst[i] = src[i];
		i++;
	}
	*dst_add = dst;
}
