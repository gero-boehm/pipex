/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 11:17:07 by gbohm             #+#    #+#             */
/*   Updated: 2023/04/26 11:23:04 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static size_t	get_token_length(char *str)
{
	size_t	i;

	i = 0;
	if (is_backslash(str[i]))
	{
		while (is_backslash(str[i]))
			i++;
		return (i);
	}
	if (is_space(str[i]))
	{
		while (is_space(str[i]))
			i++;
		return (i);
	}
	if (is_quote(str[i]))
		return (i + 1);
	while (str[i] && !is_space(str[i]) && !is_backslash(str[i])
		&& !is_quote(str[i]))
		i++;
	return (i);
}

static int	get_total_length(t_array *tokens)
{
	int				len;
	char			*token;
	unsigned long	i;

	i = 0;
	len = 0;
	while (i < tokens->size)
	{
		token = *(char **) get_element_at(tokens, i);
		len += ft_strlen(token);
		i++;
	}
	return (len);
}

int	split_str_into_tokens(char *str, t_array *tokens)
{
	size_t	len;
	char	*token;

	while (*str)
	{
		len = get_token_length(str);
		if (ft_substr2(str, 0, len, &token))
			return (free_array(tokens), 2);
		if (add_element(tokens, &token))
			return (free_array(tokens), 3);
		str += len;
	}
	return (0);
}

int	make_str_from_tokens(t_array *tokens, char **str)
{
	size_t			len;
	unsigned long	i;
	char			*cursor;
	char			*token;

	len = get_total_length(tokens);
	if (ft_malloc2(len + 1, (void **) str))
		return (1);
	i = 0;
	cursor = *str;
	while (i < tokens->size)
	{
		token = *(char **) get_element_at(tokens, i);
		while (*token)
			*cursor++ = *token++;
		i++;
	}
	*cursor = 0;
	return (0);
}
