/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jberay <jberay@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 13:48:54 by jberay            #+#    #+#             */
/*   Updated: 2024/03/12 14:13:40 by jberay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parser_loop(t_cmd *cmd, t_data *data)
{
	if (data->token[data->token_iter].type != SPACE_TOKEN)
	{
		if (data->token[data->token_iter].type == DOLLAR_TOKEN)
			parse_dollar(&cmd->cmd[data->cmds_iter], data);
		else if (data->token[data->token_iter].type == OPEN_DQUOTE_TOKEN)
			parse_dquote(&cmd->cmd[data->cmds_iter], data);
		else if (data->token[data->token_iter].type == REDIR_APPEND_TOKEN
			|| data->token[data->token_iter].type == REDIR_IN_TOKEN
			|| data->token[data->token_iter].type == REDIR_OUT_TOKEN
			|| data->token[data->token_iter].type == REDIR_HEREDOC_TOKEN)
		{
			data->cmd = cmd;
			parse_redir(&cmd->redir[data->redir_iter], data);
		}
		else
			parse_string(&cmd->cmd[data->cmds_iter], data);
		data->cmds_iter = data->cmds_iter + 1;
		ft_realloc_array(&cmd->cmd, data->cmds_iter + 1);
		ft_realloc_array(&cmd->redir, data->redir_iter + 1);
	}
	else
		data->token_iter = data->token_iter + 1;
}

/* 
	iterates through the token array 
	and create simple command array splits on pipe	*/
void	parse(t_cmd **cmd_arr, t_data *data, char *read_line)
{
	size_t	cmd_count;
	t_cmd	*cmd;

	init_data(data, read_line);
	cmd = ft_calloc(data->pipe_count + 2, sizeof(t_cmd));
	if (!cmd)
		exit (1);
	cmd_count = -1;
	while (++cmd_count < data->pipe_count + 1)
	{
		data->cmds_iter = 0;
		data->redir_iter = 0;
		cmd[cmd_count].redir = ft_calloc(2, sizeof(char *));
		cmd[cmd_count].cmd = ft_calloc(1, sizeof(char *));
		while (data->token[data->token_iter].type != EOL_TOKEN
			&& data->token[data->token_iter].type != PIPE_TOKEN)
			parser_loop(&cmd[cmd_count], data);
		cmd[cmd_count].cmd[data->cmds_iter] = NULL;
		cmd[cmd_count].redir[data->redir_iter] = NULL;
		if (data->token[data->token_iter].type == PIPE_TOKEN)
			data->token_iter = data->token_iter + 1;
	}
	cmd[cmd_count].cmd = NULL;
	cmd[cmd_count].redir = NULL;
	*cmd_arr = cmd;
}
