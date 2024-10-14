/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:18:54 by jtu               #+#    #+#             */
/*   Updated: 2024/04/19 11:29:16 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	validate_key(t_exec *exec, char *exp_arg, size_t c)
{
	char	*msg;

	if ((ft_isalnum(exp_arg[c]) == 0 && exp_arg[c] != '_'
			&& exp_arg[c] != '=') || ft_isdigit(exp_arg[0])
		|| exp_arg[0] == '=' || exp_arg[0] == '\0')
	{
		msg = ft_strjoin_3("jjsh: export: `", \
			exp_arg, "': not a valid identifier\n");
		malloc_guard(exec, msg);
		write(2, msg, ft_strlen(msg));
		return (1);
	}
	return (0);
}

int	ft_echo(t_exec *exec, char **cmd)
{
	int	i;
	int	nl;

	(void)exec;
	i = 1;
	nl = 0;
	if (!cmd[i])
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (0);
	}
	if (!ft_strncmp(cmd[i], "-n", 3))
		nl = i++;
	while (cmd[i])
	{
		ft_putstr_fd(cmd[i], STDOUT_FILENO);
		if (cmd[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (!nl)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}

int	ft_env(t_exec *exec, char **cmd)
{
	int	i;

	(void)cmd;
	i = 0;
	while (exec->envp[i])
	{
		if (ft_strrchr(exec->envp[i], '='))
			ft_putendl_fd(exec->envp[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}

int	ft_pwd(t_exec *exec, char **cmd)
{
	char	buffer[1024];
	char	*stat;
	char	*buf;
	char	*p;

	(void)cmd;
	stat = getcwd(buffer, 1024);
	if (!stat)
	{
		p = ft_strdup("PWD");
		buf = ft_getenv(exec, p);
		ft_putendl_fd(buf, STDOUT_FILENO);
		free(buf);
		return (0);
	}
	ft_putendl_fd(buffer, STDOUT_FILENO);
	return (0);
}

int	run_builtin(t_exec *exec, char **cmd)
{
	t_command_entry	command_table[7];
	int				i;

	if (!cmd[0])
		return (-1);
	command_table[0] = (t_command_entry){"cd", ft_cd};
	command_table[1] = (t_command_entry){"echo", ft_echo};
	command_table[2] = (t_command_entry){"env", ft_env};
	command_table[3] = (t_command_entry){"export", ft_export};
	command_table[4] = (t_command_entry){"pwd", ft_pwd};
	command_table[5] = (t_command_entry){"unset", ft_unset};
	command_table[6] = (t_command_entry){"exit", ft_exit};
	i = -1;
	while (++i < 7)
	{
		if (!ft_strncmp(cmd[0], command_table[i].name, \
		ft_strlen(command_table[i].name) + 1))
		{
			if (exec->cmd_count == 1)
				if (check_redirections(exec, exec->cmd[0], false))
					return (1);
			return (command_table[i].builtin(exec, cmd));
		}
	}
	return (-1);
}
