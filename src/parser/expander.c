/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 09:23:57 by jberay            #+#    #+#             */
/*   Updated: 2024/04/19 10:44:10 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	count_words(char const *s)
{
	int	word;
	int	i;

	word = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == ' ')
			i++;
		if (s[i])
			word++;
		while (s[i] && s[i] != ' ')
			i++;
	}
	return (word);
}

/*add delimiter 31 to every word or words expanded*/
static void	wrapper_31(t_exec *exec, char **astr)
{
	char	*str;
	char	*tmp;
	int		word_count;
	int		i;
	int		j;

	str = *astr;
	word_count = count_words(str);
	tmp = ft_calloc(ft_strlen(str) + word_count * 2 + 1, sizeof(char));
	malloc_guard(exec, tmp);
	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] && str[i] == ' ')
			tmp[j++] = str[i++];
		tmp[j++] = 31;
		while (str[i] && str[i] != ' ')
			tmp[j++] = str[i++];
		tmp[j++] = 31;
		while (str[i] && str[i] == ' ')
			tmp[j++] = str[i++];
	}
	*astr = tmp;
}

static bool	is_expandable(t_exec *exec, t_iterator *iter, bool is_expand)
{
	if (is_redir(&exec->token[iter->token_iter]))
		is_expand = false;
	else if (exec->token[iter->token_iter].type == STRING_TOKEN
		|| exec->token[iter->token_iter].type == SQUOTE_TOKEN)
		is_expand = true;
	return (is_expand);
}

static bool	is_in_dquote(t_exec *exec, size_t token_iter)
{
	while (token_iter >= 0)
	{
		if (exec->token[token_iter].type == OPEN_DQUOTE_TOKEN)
			return (true);
		else
			return (false);
		token_iter--;
	}
	return (false);
}

void	parse_exp_token(t_exec *exec, char **dst, \
t_iterator *iter, bool is_expand)
{
	char	*str[3];

	str[0] = ft_strdup("");
	malloc_guard(exec, str[0]);
	while (exec->token[iter->token_iter].type != EOL_TOKEN)
	{
		is_expand = is_expandable(exec, iter, is_expand);
		str[2] = str[0];
		if (exec->token[iter->token_iter].type == DOLLAR_TOKEN)
		{
			parse_dollar(exec, &str[1], iter, is_expand);
			if (is_expand && iter->token_iter > 0
				&& is_in_dquote(exec, iter->token_iter - 1))
				wrapper_31(exec, &str[1]);
		}
		else
		{
			set_start_len(exec, iter);
			parse_string(exec, &str[1], iter);
		}
		ft_strjoin_custom(&str[0], str[2], str[1]);
		malloc_guard(exec, str[0]);
	}
	*dst = str[0];
}
