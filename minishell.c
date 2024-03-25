/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jberay <jberay@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 11:41:38 by jberay            #+#    #+#             */
/*   Updated: 2024/03/25 15:27:47 by jberay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

unsigned int	g_prompt;

void	initialize_exec(t_exec *exec)
{
	exec->read_line = NULL;
	exec->token = NULL;
	exec->cmd = NULL;
	exec->pid = NULL;
	exec->pipes = NULL;
	exec->cmd_count = 0;
}

void	make_envp(t_exec *exec, char **envp)
{
	int	envp_status;

	envp_status = ft_arrdup(&exec->envp, envp);
	if (envp_status == MALLOC_ERROR)
		exit (MALLOC_ERROR);
	else if (envp_status == NULL_ERROR)
		exec->envp = NULL;
	exec->exit_code = 0;
}

void	prep_for_promt(t_exec *exec)
{
	ft_freeall_n_envp(exec);
	initialize_exec(exec);
	prompt(exec);
}

void	prompt(t_exec *exec)
{
	int	builtin_status;

	builtin_status = -1;
	if (check_command(exec))
		exec->exit_code = SYNTAX_ERROR;
	else
	{
		if (parse(exec) == 0)
		{
			if (exec->cmd_count == 1 && exec->cmd[0].cmd[0]
				&& exec->cmd[0].redir[0] == 0)
				builtin_status = run_builtin(exec, exec->cmd[0].cmd);
			if (builtin_status == -1)
				executor(exec);
			else
				exec->exit_code = builtin_status;
			g_prompt = 0;
		}
	}
	prep_for_promt(exec);
}

int	main(int argc, char **argv, char **envp)
{
	t_exec	exec;

	(void)argc;
	(void)argv;
	(void)envp;

	enablerawmode();
	togglesignal(1);
	initialize_exec(&exec);
	make_envp(&exec, envp);
	prompt(&exec);
	exit (0);
}
