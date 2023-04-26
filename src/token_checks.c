/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 10:40:20 by gbohm             #+#    #+#             */
/*   Updated: 2023/04/26 10:52:16 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft.h"

int	is_escaped(t_array *tokens, int index)
{
	char	*token;
	int		backslashes;

	if (index <= 0)
		return (0);
	token = *(char **) get_element_at(tokens, index - 1);
	backslashes = 0;
	while (is_backslash(token[backslashes]))
		backslashes++;
	return (backslashes % 2);
}

int	is_first_token_space(t_array *tokens)
{
	char			*token;

	token = *(char **) get_element_at(tokens, 0);
	return (*token == ' ');
}

int	is_last_token(t_array *tokens, unsigned long index)
{
	return (index == tokens->size - 1);
}

int	is_token_unquoted_space(char *token, char quote)
{
	return (*token == ' ' && quote == 0);
}

int	is_unescaped_quote(t_array *tokens, unsigned long index)
{
	char			*token;

	token = *(char **) get_element_at(tokens, index);
	return (is_quote(*token) && !is_escaped(tokens, index));
}
