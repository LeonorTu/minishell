/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:46:40 by jtu               #+#    #+#             */
/*   Updated: 2024/04/19 11:35:59 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*find the path of the command*/
char	*find_path(t_exec *exec, char *cmd, char **envp)
{
	char		**path;
	char		*path_cmd;
	int			i;

	while (*envp && !ft_strnstr(*envp, "PATH=", 5))
		envp++;
	if (!*envp)
		error_exit(exec, error_msg(cmd, \
			"No such file or directory", false), 127, true);
	path = ft_split(*envp + 5, ':');
	i = -1;
	path_cmd = NULL;
	while (path[++i])
	{
		path_cmd = ft_strjoin_3(path[i], "/", cmd);
		if (access(path_cmd, F_OK | X_OK) == 0)
			return (path_cmd);
		free(path_cmd);
	}
	ft_freearr(&path);
	check_cmd1(exec, cmd);
	return (0);
}

static void	run_redir_or_builtin(t_exec *exec, t_cmd parsed_cmd)
{
	int			builtin_status;

	if (check_redirections(exec, parsed_cmd, true))
		exit (1);
	if (!parsed_cmd.cmd[0])
	{
		ft_freeall(exec);
		exit(0);
	}
	builtin_status = run_builtin(exec, parsed_cmd.cmd);
	if (builtin_status != -1)
	{
		exec->exit_code = builtin_status;
		exit(builtin_status);
	}
	if (!parsed_cmd.cmd[0][0])
		error_exit(exec, error_msg(parsed_cmd.cmd[0], \
			"command not found", false), 127, true);
}

/*find the command and execute it*/
void	execute_cmd(t_exec *exec, t_cmd parsed_cmd, char **envp)
{
	char		*path;

	run_redir_or_builtin(exec, parsed_cmd);
	if (!ft_strrchr(parsed_cmd.cmd[0], '/'))
		path = find_path(exec, parsed_cmd.cmd[0], envp);
	else
	{
		check_cmd2(exec, parsed_cmd.cmd[0]);
		path = parsed_cmd.cmd[0];
	}
	if (execve(path, parsed_cmd.cmd, envp) == -1)
		error_exit(exec, error_msg(path, \
			"Permission denied", false), 126, true);
}

void	child_process(t_exec *exec)
{
	size_t	i;

	i = 0;
	if (exec->cmd_count > 1)
		create_pipes(exec);
	exec->pid = malloc(sizeof(int) * (exec->cmd_count));
	while (i < exec->cmd_count)
	{
		exec->pid[i] = fork();
		if (exec->pid[i] < 0)
			error_exit(exec, error_msg(NULL, strerror(errno), false), 1, true);
		togglesignal(IGNORE);
		if (exec->pid[i] == 0)
		{
			togglesignal(DEFAULT);
			togglerawmode(0);
			if (exec->cmd_count > 1)
				pipe_dup(i, exec);
			execute_cmd(exec, exec->cmd[i], exec->envp);
		}
		i++;
	}
	close_pipes(exec);
}

/*get the command from parser and create pipes and child process*/
void	executor(t_exec *exec)
{
	size_t	i;
	int		status;

	child_process(exec);
	i = -1;
	while (++i < exec->cmd_count)
		waitpid(exec->pid[i], &status, 0);
	togglesignal(HANDLER);
	exec->exit_code = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		else if (WTERMSIG(status) == SIGINT)
			ft_putstr_fd("\n", STDERR_FILENO);
		exec->exit_code = WTERMSIG(status);
	}
}
