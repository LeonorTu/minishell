/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_iter.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jberay <jberay@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 09:20:31 by jberay            #+#    #+#             */
/*   Updated: 2024/03/21 12:00:37 by jberay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHAR_ITER_H
# define CHAR_ITER_H

# include <stdlib.h>
# include <stdbool.h>

typedef struct s_char_iter
{
	char	*start;
	char	*end;
}	t_char_iter;

/* constructor */
t_char_iter		char_iter_constructor(char *start, size_t	len);

/* returns a pointer to the current location of iterator's cursor */
char			*char_iter_cursor(t_char_iter *self);

char			char_iter_peek_next(t_char_iter *self);

/* returns quotes*/
char			*char_find_dq(t_char_iter *self);

/*	
	peek and return next character
	do not advance the cursor
	check for null 
*/
char			char_iter_peek(t_char_iter *self);

/* read next character and advances cursor check for null */
char			char_iter_next(t_char_iter *self);

#endif