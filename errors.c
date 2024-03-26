/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jberay <jberay@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 11:10:45 by jberay            #+#    #+#             */
/*   Updated: 2024/03/26 10:37:13 by jberay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_error(t_exec *exec, char *msg, int ret)
{
	ft_putstr_fd("jjsh-1.0$ ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	exec->exit_code = ret;
	ft_freeall(exec);
	if (ret == MALLOC_ERROR)
		exit(1);
	return (ret);
}
