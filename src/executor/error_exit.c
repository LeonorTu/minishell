/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtu <jtu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 20:50:50 by jtu               #+#    #+#             */
/*   Updated: 2024/04/19 10:43:36 by jtu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_strjoin_3(char const *s1, char const *s2, char const *s3)
{
	int		i;
	int		j;
	char	*joined_str;

	i = 0;
	j = 0;
	joined_str = malloc(ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1);
	if (!joined_str)
		return (NULL);
	while (s1[i])
		joined_str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		joined_str[j++] = s2[i++];
	i = 0;
	while (s3[i])
		joined_str[j++] = s3[i++];
	joined_str[j] = '\0';
	return (joined_str);
}

char	*error_msg(char *cmd, char *msg, bool cd)
{
	char	*err_msg;
	char	*temp;

	temp = ft_strjoin_3(": ", msg, "\n");
	if (cd)
		err_msg = ft_strjoin_3("jjsh: cd: ", cmd, temp);
	else
		err_msg = ft_strjoin_3("jjsh: ", cmd, temp);
	free(temp);
	return (err_msg);
}

int	error_exit(t_exec *exec, char *err_msg, int code, bool quit)
{
	write(STDERR_FILENO, err_msg, ft_strlen(err_msg));
	free(err_msg);
	ft_freeall_n_envp(exec);
	if (quit)
		exit(code);
	return (1);
}
