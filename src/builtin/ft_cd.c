/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 15:56:50 by jtu               #+#    #+#             */
/*   Updated: 2024/04/19 11:28:06 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*find_home(t_exec *exec, char **envp)
{
	char	*home_path;

	while (*envp && !ft_strnstr(*envp, "HOME=", 5))
		envp++;
	if (!*envp)
		error_exit(exec, error_msg(NULL, strerror(errno), true), 1, false);
	home_path = *envp + 5;
	return (home_path);
}
/*
	oldpwd gets value from pwd env variable
	if pwd is unset
	old pwd is set to empty
	if cd again it gets the pwd getwcd
*/

void	update_pwd(t_exec *exec, char *buffer)
{
	char	*old[3];
	char	*key;
	char	*pwd;

	old[0] = "export";
	old[2] = NULL;
	old[1] = ft_strjoin("OLDPWD=", buffer);
	malloc_guard(exec, old[1]);
	ft_export(exec, old);
	free(old[1]);
	key = ft_strdup("PWD");
	malloc_guard(exec, key);
	pwd = ft_getenv(exec, key);
	if (pwd != NULL)
	{
		getcwd(buffer, 1024);
		old[1] = ft_strjoin("PWD=", buffer);
		malloc_guard(exec, old[1]);
		ft_export(exec, old);
	}
	free(pwd);
}

int	cd_stat(t_exec *exec, char *dir)
{
	struct stat	buf;

	if (!stat(dir, &buf))
	{
		if (S_ISDIR(buf.st_mode) && chdir(dir) == -1)
			return (error_exit(exec, error_msg(dir, \
				"Permission denied", true), 126, false));
		else if (!S_ISDIR(buf.st_mode))
			return (error_exit(exec, error_msg(dir, \
				"Not a directory", true), 127, false));
		else
			return (0);
	}
	else
		return (error_exit(exec, error_msg(dir, \
			"No such file or directory", true), 126, false));
}

int	ft_cd(t_exec *exec, char **cmd)
{
	char		buffer[1024];
	char		*home;
	char		*err_msg;
	char		*dir;

	getcwd(buffer, 1024);
	dir = cmd[1];
	if (ft_arrlen(cmd) < 2 || !ft_strncmp(cmd[1], "~", 2))
	{
		home = ft_strdup("HOME");
		dir = ft_getenv(exec, home);
		if (!dir)
		{
			err_msg = ft_strdup("cd: HOME not set\n");
			return (error_exit(exec, err_msg, 1, false));
		}
	}
	if (cd_stat(exec, dir))
		return (1);
	if (ft_arrlen(cmd) < 2 || !ft_strncmp(cmd[1], "~", 2))
		free(dir);
	update_pwd(exec, buffer);
	return (0);
}
