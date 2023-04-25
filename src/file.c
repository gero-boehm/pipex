/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:14:04 by gbohm             #+#    #+#             */
/*   Updated: 2023/04/23 13:28:41 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include "pipex.h"
#include "libft.h"
#include "get_next_line.h"
#include "ft_printf.h"

int	get_infile(char *fspath, int *fd)
{
	*fd = -1;
	if (access(fspath, F_OK))
		return (ft_fdprintf(2, "pipex: %s: no such file or directory\n"
				, fspath), 1);
	if (access(fspath, R_OK))
		return (ft_fdprintf(2, "pipex: %s: Permission denied\n", fspath), 2);
	*fd = open(fspath, O_RDONLY, 0644);
	return (0);
}

int	get_write_fd(char *fspath, int append, int *fd)
{
	int	flags;

	flags = O_WRONLY | O_CREAT | O_APPEND;
	if (!append)
		flags |= O_TRUNC;
	*fd = open(fspath, flags, 0644);
	return (*fd == -1);
}

int	output(char *file, int fd, int is_here_doc)
{
	int		outfile;
	char	*line;
	size_t	len;

	if (get_write_fd(file, is_here_doc, &outfile))
		return (1);
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		len = ft_strlen(line);
		write(outfile, line, len);
		free(line);
	}
	close(outfile);
	return (0);
}
