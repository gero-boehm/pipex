/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 00:15:22 by gbohm             #+#    #+#             */
/*   Updated: 2023/04/25 17:24:09 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "pipex.h"
#include "libft.h"
#include "ft_printf.h"

int	is_backslash(char c)
{
	return (c == '\\');
}

int	is_space(char c)
{
	return (c == ' ');
}

size_t	get_token_length(char *str)
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

int	get_total_length(t_array *tokens)
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

int	reduce_quotes(t_array *tokens, int start, int end, t_array *reduced)
{
	char	*token;
	char	quote;

	quote = 0;
	while (start < end)
	{
		token = *(char **) get_element_at(tokens, start);
		if (is_quote(*token) && !is_escaped(tokens, start))
		{
			if (quote == 0)
			{
				quote = *token;
				start++;
				continue ;
			}
			else if (*token == quote)
			{
				quote = 0;
				start++;
				continue ;
			}
		}
		if (is_backslash(*token) && quote != '\'')
		{
			if (reduce_backslashes(&token))
				return (1);
			if (*token == 0)
			{
				start++;
				continue ;
			}
		}
		if (add_element(reduced, &token))
			return (2);
		start++;
	}
	return (0);
}

int	make_segment(t_array *tokens, int start, int end, t_array *splits)
{
	t_array	reduced;
	char	*str;

	if (create_array(&reduced, sizeof(char *)))
		return (1);
	if (reduce_quotes(tokens, start, end, &reduced))
		return (free_str_array(&reduced), 2);
	if (make_str_from_tokens(&reduced, &str))
		return (free_str_array(&reduced), 3);
	if (add_element(splits, &str))
		return (free_str_array(&reduced), 4);
	free_array(&reduced);
	return (0);
}

// int	make_segment(t_array *tokens, int start, int end, t_array *splits)
// {
// 	int		len;
// 	int		i;
// 	char	*token;
// 	char	*str;

// 	len = get_total_length(tokens, start, end);
// 	if (ft_malloc2(len + 1, (void **) &str))
// 		return (1);
// 	i = 0;
// 	while (start < end)
// 	{
// 		token = *(char **) get_element_at(tokens, start);
// 		while (*token)
// 			str[i++] = *token++;
// 		start++;
// 	}
// 	str[len] = 0;
// 	if (add_element(splits, &str))
// 		return (2);
// 	return (0);
// }

int	is_first_tokens_space(t_array *tokens)
{
	char			*token;

	token = *(char **) get_element_at(tokens, 0);
	return (*token == ' ');
}

int is_last_token(t_array *tokens, unsigned long index)
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

int	combine_tokens(t_array *tokens, t_array *splits)
{
	char			quote;
	int				start;
	char			*token;
	unsigned long	i;
	unsigned long	end;

	quote = 0;
	start = 0;
	i = 0;
	while (i < tokens->size)
	{
		token = *(char **) get_element_at(tokens, i);
		if (is_first_tokens_space(tokens))
			start++;
		else if (is_last_token(tokens, i) || is_token_unquoted_space(token, quote))
		{
			end = i;
			if (is_last_token(tokens, i) && !is_space(*token))
				end++;
			if (make_segment(tokens, start, end, splits))
				return (1);
			start = i + 1;
		}
		if (is_quote(*token) && !is_escaped(tokens, i))
		{
			if (quote == 0)
				quote = *token;
			else if (*token == quote)
				quote = 0;
		}
		i++;
	}
	return (quote != 0);
}

int	convert_dynamic_array(t_array *splits, char ***params)
{
	size_t	len;

	len = splits->size * splits->bytes;
	if (ft_malloc2(len + sizeof(char *), (void **) params))
		return (1);
	ft_memcpy(*params, splits->elements, len);
	(*params)[splits->size] = NULL;
	return (0);
}

int	split_params(char *cmd, char ***params)
{
	t_array	tokens;
	t_array	splits;

	if (create_array(&tokens, sizeof(char *)))
		return (1);
	if (create_array(&splits, sizeof(char *)))
		return (free_str_array(&tokens), 2);
	if (split_str_into_tokens(cmd, &tokens))
		return (free_str_array(&tokens), free_str_array(&splits), 3);
	// for (size_t i = 0; i < tokens.size; i++)
	// 	ft_fdprintf(2, "'%s'\n", *(char **) get_element_at(&tokens, i));
	if (combine_tokens(&tokens, &splits))
		return (free_str_array(&tokens), free_str_array(&splits), 4);
	free_str_array(&tokens);
	// char *null = NULL;
	// add_element(&splits, &null);
	// char **strs = splits.elements;


	// while (*strs != NULL)
	// 	ft_fdprintf(2, "== %s\n", *strs++);

	if (convert_dynamic_array(&splits, params))
		return (free_str_array(&splits), 5);
	return (0);
}
