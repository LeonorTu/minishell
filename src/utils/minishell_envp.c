/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_envp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 09:54:13 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 11:38:29 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
if overflow int max and int min and negative return 0
bash-3.2$ bash
bash: warning: shell level (2147483647) too high, resetting to 1
*/

int	is_shlvl_overflow(char *shlvl, int lvl)
{
	if (lvl > INT_MAX && lvl < 0)
		return (0);
	else if (lvl > 1000 && lvl < 2147483647)
	{
		ft_putstr_fd("jjsh: warning: shell level (", 2);
		ft_putstr_fd(shlvl, 2);
		ft_putstr_fd(") too high, resetting to 1\n", 2);
		return (1);
	}
	else
		return (lvl);
}

int	ft_isstrdigit(char *str)
{
	int	i;

	i = -1;
	while ((str[++i]))
	{
		if (!ft_isdigit(str[i]))
			return (0);
	}
	return (1);
}

char	*make_shlvl(t_exec *exec, char *shlvl)
{
	char	*tmp;
	char	*join;
	int		lvl;

	if (!shlvl || !ft_isstrdigit(shlvl))
	{
		tmp = ft_strdup("SHLVL=1");
		malloc_guard(exec, tmp);
		return (tmp);
	}
	lvl = ft_atoi(shlvl);
	lvl = is_shlvl_overflow(shlvl, ++lvl);
	join = ft_itoa(lvl);
	malloc_guard(exec, join);
	tmp = ft_strjoin("SHLVL=", join);
	free(join);
	malloc_guard(exec, tmp);
	return (tmp);
}

void	update_shlvl(t_exec *exec)
{
	char	*shlvl;
	char	*cmd[3];

	cmd[0] = "export";
	cmd[2] = NULL;
	shlvl = getenv("SHLVL");
	cmd[1] = make_shlvl(exec, shlvl);
	ft_export(exec, cmd);
	free(cmd[1]);
}

void	make_envp(t_exec *exec, char **envp)
{
	int		envp_status;
	char	buffer[1024];
	char	*pwd;

	envp_status = ft_arrdup(&exec->envp, envp);
	if (envp_status == MALLOC_ERROR)
		exit (MALLOC_ERROR);
	else if (envp_status == NULL_ERROR)
		exec->envp = NULL;
	exec->exit_code = 0;
	update_shlvl(exec);
	if (isatty(0))
	{
		getcwd(buffer, 1024);
		ft_unset(exec, (char *[]){"unset", "OLDPWD", NULL});
		pwd = ft_strjoin("PWD=", buffer);
		malloc_guard(exec, pwd);
		ft_export(exec, (char *[]){"export", pwd, NULL});
	}
}
