/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 00:15:22 by gbohm             #+#    #+#             */
/*   Updated: 2023/04/26 11:21:09 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "ft_printf.h"

static int	reduce_quotes(t_array *tokens, int start, int end, t_array *reduced)
{
	char	*token;
	char	quote;

	quote = 0;
	while (start < end)
	{
		token = *(char **) get_element_at(tokens, start);
		if (is_quote(*token) && !is_escaped(tokens, start))
		{
			if (toggle_quotes(&quote, *token))
			{
				start++;
				continue ;
			}
		}
		if (is_backslash(*token) && quote != '\'')
		{
			if (reduce_backslashes(&token))
				return (1);
		}
		if (add_element(reduced, &token))
			return (2);
		start++;
	}
	return (0);
}

static int	make_segment(t_array *tokens, int start, int end, t_array *params)
{
	t_array	reduced;
	char	*str;

	if (create_array(&reduced, sizeof(char *)))
		return (1);
	if (reduce_quotes(tokens, start, end, &reduced))
		return (free_str_array(&reduced), 2);
	if (make_str_from_tokens(&reduced, &str))
		return (free_str_array(&reduced), 3);
	if (add_element(params, &str))
		return (free_str_array(&reduced), 4);
	free_array(&reduced);
	return (0);
}

static int	process_end_of_segment(t_array *tokens, unsigned long start,
	unsigned long end, t_array *params)
{
	char	*token;

	token = *(char **) get_element_at(tokens, end);
	if (is_last_token(tokens, end) && !is_space(*token))
		end++;
	if (make_segment(tokens, start, end, params))
		return (1);
	return (0);
}

static int	combine_tokens(t_array *tokens, t_array *params)
{
	char			quote;
	int				start;
	char			*token;
	unsigned long	i;

	quote = 0;
	start = 0;
	i = 0;
	while (i < tokens->size)
	{
		token = *(char **) get_element_at(tokens, i);
		if (is_first_token_space(tokens))
			start++;
		else if (is_last_token(tokens, i)
			|| is_token_unquoted_space(token, quote))
		{
			if (process_end_of_segment(tokens, start, i, params))
				return (1);
			start = i + 1;
		}
		if (is_quote(*token) && !is_escaped(tokens, i))
			toggle_quotes(&quote, *token);
		i++;
	}
	return (quote != 0);
}

int	split_params(char *cmd, t_array *params)
{
	t_array		tokens;
	const char	*null = NULL;

	if (create_array(&tokens, sizeof(char *)))
		return (1);
	if (split_str_into_tokens(cmd, &tokens))
		return (free_str_array(&tokens), 2);
	if (combine_tokens(&tokens, params))
		return (free_str_array(&tokens), 3);
	free_str_array(&tokens);
	if (add_element(params, &null))
		return (free_str_array(&tokens), 4);
	return (0);
}
