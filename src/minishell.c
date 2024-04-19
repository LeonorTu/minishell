/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:41:38 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 10:44:23 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_signal;

void	initialize_exec(t_exec *exec)
{
	exec->read_line = NULL;
	exec->token = NULL;
	exec->cmd = NULL;
	exec->pid = NULL;
	exec->pipes = NULL;
	exec->cmd_count = 0;
}

void	prep_for_promt(t_exec *exec)
{
	togglerawmode(HANDLER);
	ft_freeall_n_envp(exec);
	initialize_exec(exec);
	prompt(exec);
}

void	run_cmd(t_exec *exec)
{
	int	builtin_status;
	int	std_in;
	int	std_out;

	builtin_status = -1;
	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	if (exec->cmd_count == 1 && exec->cmd[0].cmd[0])
		builtin_status = run_builtin(exec, exec->cmd[0].cmd);
	if (builtin_status == -1)
	{
		close(std_in);
		close(std_out);
		executor(exec);
	}
	else
		exec->exit_code = builtin_status;
	dup2(std_in, STDIN_FILENO);
	dup2(std_out, STDOUT_FILENO);
	close (std_in);
	close(std_out);
}

void	prompt(t_exec *exec)
{
	exec->read_line = readline ("jjsh-1.0$ ");
	if (!exec->read_line)
	{
		if (isatty(0))
			write(2, "exit\n", 5);
		ft_freeall(exec);
		exit (0);
	}
	if (g_signal)
		exec->exit_code = 1;
	g_signal = 0;
	if (*exec->read_line != '\0')
	{
		if (ft_addhistory(exec) == 0)
		{
			if (expander(exec))
				exec->exit_code = SYNTAX_ERROR;
			else if (*exec->read_line != '\0')
				if (parse(exec) == 0)
					run_cmd(exec);
		}
	}
	prep_for_promt(exec);
}

int	main(int argc, char **argv, char **envp)
{
	t_exec	exec;

	if (argc != 1 || argv[1])
	{
		write(2, "jjsh-1.0: too many arguments\n", 29);
		exit (127);
	}
	togglesignal(HANDLER);
	togglerawmode(HANDLER);
	initialize_exec(&exec);
	make_envp(&exec, envp);
	if (isatty(0))
		make_history(&exec);
	prompt(&exec);
	exit (0);
}
