/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 11:19:04 by gbohm             #+#    #+#             */
/*   Updated: 2023/04/26 11:22:46 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "pipex.h"

void	free_str_array(t_array *strs)
{
	unsigned long	i;
	char			*str;

	i = 0;
	while (i < strs->size)
	{
		str = *(char **) get_element_at(strs, i);
		free(str);
		i++;
	}
	free_array(strs);
}

int	reduce_backslashes(char **str)
{
	size_t	len;

	len = ft_strlen(*str) / 2;
	if (ft_chrrep2('\\', len, str))
		return (1);
	return (0);
}

int	toggle_quotes(char *quote, char token)
{
	if (*quote == 0)
	{
		*quote = token;
		return (1);
	}
	else if (token == *quote)
	{
		*quote = 0;
		return (1);
	}
	return (0);
}
