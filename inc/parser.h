/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jberay <jberay@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:53:49 by jberay            #+#    #+#             */
/*   Updated: 2024/03/26 10:27:56 by jberay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef struct s_iterator
{
	size_t	token_iter;
	size_t	cmds_iter;
	size_t	redir_iter;
	size_t	cmd_count;
}	t_iterator;

/*parser utils*/
int		ft_substr_custom(char **dst, char const *s, char *start, size_t len);
int		ft_strjoin_custom(char **dst, char *s1, char *s2);

#endif