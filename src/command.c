/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 13:33:08 by gbohm             #+#    #+#             */
/*   Updated: 2023/04/20 18:23:11 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "pipex.h"
#include "libft.h"
#include "ft_printf.h"

static int	starts_with(char *str, char *match)
{
	while (*match)
	{
		if (*str++ != *match++)
			return (0);
	}
	return (1);
}

static int	is_local(char *cmd)
{
	return (!access(cmd, F_OK));
}

static int	get_paths(char *const *envp, char ***paths)
{
	char	*str;

	*paths = NULL;
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

static int	build_path(char *dir, char *cmd, char **path)
{
	if (ft_sprintf(path, "%s/%s", dir, cmd) == -1)
		return (1);
	return (0);
}

int	get_command_path(char *cmd, char *const *envp, char **path)
{
	char	**paths;
	char	**cursor;

	if (is_local(cmd))
	{
		*path = cmd;
		return (0);
	}
	if (get_paths(envp, &paths))
		return (1);
	cursor = paths;
	while (*cursor)
	{
		if (build_path(*cursor++, cmd, path))
			return (ft_free2d((void **) paths), 2);
		if (access(*path, F_OK))
		{
			free(*path);
			continue ;
		}
		ft_free2d((void **) paths);
		return (0);
	}
	ft_free2d((void **) paths);
	return (3);
}
