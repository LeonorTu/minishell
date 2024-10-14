# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/07 11:38:23 by jberay            #+#    #+#              #
#    Updated: 2024/04/19 11:18:06 by jtu              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN = \033[0;92m

NAME			=	minishell

CC				=	cc
CFLAGS			=	-Wall -Wextra -Werror -fsanitize=address -g -static-libsan
RM				=	rm -rf
READLINE_HEADER = ~/.brew/opt/readline/include
READLINE_LIB = ~/.brew/opt/readline/lib

SRCS 			=	src/minishell.c \
					src/utils/minishell_utils.c \
					src/utils/minishell_envp.c \
					src/utils/minishell_history.c \
					src/parser/heredoc.c \
					src/parser/heredoc_utils.c \
					src/parser/heredoc_tokenizer.c \
					src/utils/char_iter.c \
					src/parser/expander.c \
					src/token/tokenizer.c \
					src/token/tokenizer_strs.c \
					src/token/tokenizer_redir.c \
					src/token/tokenizer_utils.c \
					src/token/tokenizer_syntax.c \
					src/parser/parser.c \
					src/parser/parser_utils.c \
					src/parser/parser_tokenstrs.c \
					src/parser/parser_tokendollar.c \
					src/signals.c \
					src/utils/array_utils.c \
					src/utils/errors.c \
					src/utils/frees.c \
					src/executor/executor.c \
					src/executor/redirections.c \
					src/executor/error_exit.c \
					src/executor/pipes.c \
					src/executor/check_cmd.c \
					src/builtin/builtins.c \
					src/builtin/ft_cd.c \
					src/builtin/ft_unset.c \
					src/builtin/ft_export.c \
					src/builtin/ft_exit.c \


OBJS			=	$(SRCS:%.c=%.o)

LIBFT_PATH		=	src/libft
LIBFT			=	$(LIBFT_PATH)/libft.a

all:				$(NAME)

$(NAME):			$(LIBFT) $(OBJS)
					@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -lhistory -L $(READLINE_LIB) -I$(READLINE_HEADER)  -o $(NAME)
					@echo "$(GREEN)Minishell compiled!$(DEF_COLOR)"

%.o:%.c
					@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
					@make -C $(LIBFT_PATH) all

clean:
					@make -C $(LIBFT_PATH) clean
					@$(RM) $(OBJS)

fclean:				clean
					@make -C $(LIBFT_PATH) fclean
					@$(RM) $(NAME)

re:					fclean all

.PHONY:				all bonus clean fclean re libft