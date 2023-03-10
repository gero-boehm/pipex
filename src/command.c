/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 13:33:08 by gbohm             #+#    #+#             */
/*   Updated: 2023/03/14 17:53:24 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "pipex.h"
#include "libft.h"
#include "ft_printf.h"

static int	starts_with(const char *str, const char *match)
{
	while (*match)
	{
		if (*str++ != *match++)
			return (0);
	}
	return (1);
}

static int	get_paths(char *const *envp, char ***paths)
{
	char	*str;

	while (*envp)
	{
		if (!starts_with(*envp, "PATH="))
		{
			envp++;
			continue ;
		}
		if (ft_substr2(*envp, 5, ft_strlen(*envp) - 5, &str))
			return (1);
		if (*str == 0)
			return (2);
		if (ft_split2(str, ':', paths))
			return (free(str), 3);
		free(str);
		break ;
	}
	return (*paths == NULL);
}

int	build_path2(char *dir, const char *cmd, char **path)
{
	if (starts_with(cmd, "/") || starts_with(cmd, "./")
		|| starts_with(cmd, "../"))
	{
		if (ft_strdup2(cmd, path))
			return (1);
	}
	else if (ft_sprintf(path, "%s/%s", dir, cmd) == -1)
		return (2);
	return (0);
}

int	get_command_path2(const char *cmd, char *const *envp, char **path)
{
	char	**paths;
	char	**cursor;

	paths = NULL;
	if (get_paths(envp, &paths))
		return (1);
	cursor = paths;
	while (*cursor)
	{
		if (build_path2(*cursor, cmd, path))
			return (ft_arrfree((void **) paths), 2);
		cursor++;
		if (access(*path, F_OK))
		{
			free(*path);
			continue ;
		}
		ft_arrfree((void **) paths);
		return (0);
	}
	ft_arrfree((void **) paths);
	return (3);
}
