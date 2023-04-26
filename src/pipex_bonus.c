/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:44:18 by gbohm             #+#    #+#             */
/*   Updated: 2023/04/26 17:14:10 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "get_next_line.h"
#include "pipex.h"
#include "libft.h"
#include "ft_printf.h"

static int	is_eof(char *line, char *eof)
{
	size_t			len;
	unsigned long	i;

	len = ft_strlen(eof);
	i = 0;
	while (i < len)
	{
		if (line[i] != eof[i])
			return (0);
		i++;
	}
	return (line[i] == '\n');
}

static int	get_here_doc(char *eof)
{
	char	*line;
	int		fd;

	if (get_write_fd("here_doc", 0, &fd))
		return (1);
	while (1)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(STDIN);
		if (line == NULL || is_eof(line, eof))
			break ;
		write(fd, line, ft_strlen(line));
		free(line);
	}
	free(line);
	close(fd);
	return (0);
}

int	main(int argc, char **argv, char *const *envp)
{
	int	is_here_doc;
	int	infile;

	if (envp == NULL)
		return (1);
	if (argc < 2)
		return (ft_fdprintf(2, "pipex: incorrect argument count\n"), 2);
	is_here_doc = ft_streq(argv[1], "here_doc");
	if (is_here_doc)
	{
		if (argc < 4)
			return (ft_fdprintf(2, "pipex: must have at least here_doc, "
					"limiter and outfile\n"), 3);
		if (get_here_doc(argv[2]))
			return (4);
	}
	else if (argc < 3)
		return (ft_fdprintf(2, "pipex: must have at least "
				"infile and outfile\n"), 5);
	get_infile(argv[1], &infile);
	run(infile, is_here_doc, argv, envp);
	unlink("here_doc");
	return (0);
}
