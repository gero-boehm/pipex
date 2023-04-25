/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:12:05 by gbohm             #+#    #+#             */
/*   Updated: 2023/04/25 16:51:46 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "pipex.h"
#include "libft.h"
#include "ft_printf.h"

static int	get_argc(char **argv)
{
	int	count;

	count = 0;
	while (argv[count] != NULL)
		count++;
	return (count);
}

void p(char **params)
{
	while (*params != NULL)
		ft_fdprintf(2, "params: %s\n", *params++);
	ft_fdprintf(2, "===\n");
}

static void	run_child(char *cmd, char *const *envp, int fd, int ports[2])
{
	char	**params;
	char	*path;

	close(ports[0]);
	dup2(fd, STDIN);
	dup2(ports[1], STDOUT);
	close(fd);
	close(ports[1]);
	if (fd == -1)
		return ;
	if (split_params(cmd, &params))
	{
		ft_fdprintf(2, "pipex: invalid parameters\n");
		ft_free2d((void **) params);
		exit(1);
	}
	if (get_command_path(params[0], envp, &path))
	{
		ft_fdprintf(2, "pipex: %s: command not found\n", params[0]);
		ft_free2d((void **) params);
		exit(2);
	}
	execve(path, params, envp);
	ft_fdprintf(2, "pipex: failed to execute command\n");
	free(path);
	// ft_free2d((void **) params);
	exit(3);
}

static void	run_parent(int *fd, int ports[2])
{
	close(ports[1]);
	close(*fd);
	*fd = ports[0];
}

int	run(int fd, int is_here_doc, char **argv, char *const *envp)
{
	int		i;
	int		ports[2];
	pid_t	pid;
	int		argc;

	argc = get_argc(argv);
	i = is_here_doc + 2;
	while (i < argc - 1)
	{
		if (pipe(ports) == -1)
			return (1);
		pid = fork();
		if (pid == -1)
			return (2);
		if (pid == 0)
			run_child(argv[i], envp, fd, ports);
		else
			run_parent(&fd, ports);
		i++;
	}
	while (i-- - is_here_doc - 2)
		waitpid(0, NULL, 0);
	if (output(argv[argc - 1], fd, is_here_doc))
		return (ft_fdprintf(2, "pipex: error writing to outfile\n"), 2);
	close(fd);
	return (0);
}
