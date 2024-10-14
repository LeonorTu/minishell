/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:13:23 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 11:18:40 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <errno.h>

# include <readline/readline.h>
# include <readline/history.h>
# include "../src/libft/libft.h"
# include "token.h"
# include "parser.h"
# include "char_iter.h"

# include <signal.h>
# include <termios.h>

# define RD 0
# define WR 1

extern int	g_signal;

typedef struct s_cmd
{
	char	**cmd;
	char	**redir;
}	t_cmd;

typedef struct s_exec
{
	char	*read_line;
	t_token	*token;
	t_cmd	*cmd;
	size_t	cmd_count;
	char	**envp;
	int		exit_code;
	int		*pid;
	int		**pipes;
	int		history_fd;
}	t_exec;

typedef int	(*t_builtin)(t_exec *, char **);

typedef struct s_command_entry
{
	char		*name;
	t_builtin	builtin;
}	t_command_entry;

typedef enum e_error_code
{
	NO_ERR,
	FORK_ERROR,
	MALLOC_ERROR = -1,
	NULL_ERROR = -2,
	SYNTAX_ERROR = 258,
}	t_error_code;

typedef enum e_signal
{
	DEFAULT,
	HANDLER,
	HEREDOC,
	IGNORE
}	t_signal;

/*minishell*/
void	make_envp(t_exec *exec, char **envp);
void	make_history(t_exec *exec);
int		ft_addhistory(t_exec *exec);
int		heredoc(t_exec *exec, char **dst, t_iterator *iter, bool is_expand);
void	write_to_heredoc(t_exec *exec, int fd, \
char *delimiter, int is_expand);
void	heredoc_tokenizer(t_exec *exec);

/*minishell utils*/
void	prep_for_promt(t_exec *exec);
void	prompt(t_exec *exec);
void	rl_replace_line(const char *text, int clear_undo);
void	initialize_exec(t_exec *exec);
int		check_command(t_exec *exec);
int		expander(t_exec *exec);
void	parse_exp_token(t_exec *exec, char **dst, \
t_iterator *iter, bool is_expand);
void	set_start_len(t_exec *exec, t_iterator *iter);
void	malloc_guard(t_exec *exec, void *ptr);

/*signals*/
void	togglerawmode(int mode);
void	togglesignal(t_signal mode);
void	signal_handler(int signum);

/* token */
void	tokenizer(t_exec *exec);

/* parser */
int		parse(t_exec *exec);
void	parse_redir(t_exec *exec, char **dst, t_iterator *iter);
void	parse_string(t_exec *exec, char **dst, t_iterator *iter);
void	parse_dollar(t_exec *exec, char **dst, \
t_iterator *iter, bool is_expand);
void	parse_dquote(t_exec *exec, char **dst, \
t_iterator *iter, bool is_expand);
void	init_data(t_exec *exec, t_iterator *iter);

/*parser utils*/
bool	is_redir(t_token *token);
char	*ft_getenv(t_exec *exec, char *key);

/*Array utils*/
size_t	ft_arrlen(char **arr);
int		ft_arrdup(char ***dst_add, char **src);
void	ft_arrcpy(char ***dst_add, char **src);
int		ft_realloc_array(char ***dst_add, size_t size);

/*frees*/
void	ft_freearr(char ***array_add);
void	ft_freestruct(t_cmd **cmd);
void	ft_freeall(t_exec *exec);
void	ft_freeall_n_envp(t_exec *exec);

int		ft_error(t_exec *exec, char *msg, int return_code);

/*exector*/
void	executor(t_exec *exec);
int		error_exit(t_exec *exec, char *err_msg, int code, bool quit);
char	*error_msg(char *s, char *msg, bool cd);

void	free_arr(char **arr);
void	error_free_exit(char **s);
char	*ft_strjoin_3(char const *s1, char const *s2, char const *s3);
int		check_redirections(t_exec *exec, t_cmd parsed_cmd, bool quit);
void	close_pipes(t_exec *exec);
void	pipe_dup(size_t i, t_exec *exec);
void	create_pipes(t_exec *exec);
void	check_cmd1(t_exec *exec, char *cmd);
void	check_cmd2(t_exec *exec, char *cmd);

/*builtins*/
int		run_builtin(t_exec *exec, char **cmd);
int		ft_unset(t_exec *exec, char **cmd);
int		ft_cd(t_exec *exec, char **cmd);
int		ft_export(t_exec *exec, char **cmd);
int		ft_exit(t_exec *exec, char **cmd);
int		ft_pwd(t_exec *exec, char **cmd);
int		ft_env(t_exec *exec, char **cmd);
int		ft_echo(t_exec *exec, char **cmd);
int		validate_key(t_exec *exec, char *exp_arg, size_t c);

/*debug*/
void	token_print(t_token *token);
void	print_array(char **array);
void	print_cmd(t_cmd **cmd_ptr);
#endif