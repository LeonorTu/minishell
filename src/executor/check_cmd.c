/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 10:49:34 by jtu               #+#    #+#             */
/*   Updated: 2024/04/19 10:43:36 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	check_cmd1(t_exec *exec, char *cmd)
{
	struct stat	buf;

	if (access(cmd, F_OK) != 0)
		error_exit(exec, error_msg(cmd, "command not found", false), 127, true);
	if (stat(cmd, &buf) == 0)
	{
		if (S_ISREG(buf.st_mode))
			error_exit(exec, error_msg(cmd, "command not found", \
				false), 127, true);
		else if (S_ISDIR(buf.st_mode))
			error_exit(exec, error_msg(cmd, "command not found", \
				false), 127, true);
	}
	else
		error_exit(exec, error_msg(cmd, "Permission denied", false), 126, true);
}

void	check_cmd2(t_exec *exec, char *cmd)
{
	struct stat	buf;

	if (access(cmd, F_OK) != 0)
		error_exit(exec, error_msg(cmd, \
			"No such file or directory", false), 127, true);
	if (access(cmd, X_OK) != 0)
		error_exit(exec, error_msg(cmd, "Permission denied", false), 126, true);
	if (stat(cmd, &buf) == 0)
	{
		if (S_ISDIR(buf.st_mode))
			error_exit(exec, error_msg(cmd, \
				"is a directory", false), 126, true);
	}
	else
		error_exit(exec, error_msg(cmd, "Permission denied", false), 126, true);
}
