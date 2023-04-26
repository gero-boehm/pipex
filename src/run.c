/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:12:05 by gbohm             #+#    #+#             */
/*   Updated: 2023/04/26 15:30:30 by gbohm            ###   ########.fr       */
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

static void	exit_with_error(char *error, char *extra, t_array *params)
{
	ft_fdprintf(2, error, extra);
	free_str_array(params);
	exit(3);
}

static void	run_child(char *cmd, char *const *envp, int fd, int ports[2])
{
	t_array	params;
	char	*path;
	char	*first_param;

	close(ports[0]);
	dup2(fd, STDIN);
	dup2(ports[1], STDOUT);
	close(fd);
	close(ports[1]);
	if (fd == -1)
		return ;
	if (create_array(&params, sizeof(char *)))
		exit(1);
	if (split_params(cmd, &params))
		exit_with_error("pipex: invalid parameters\n", NULL, &params);
	first_param = *(char **) get_element_at(&params, 0);
	if (get_command_path(first_param, envp, &path))
		exit_with_error("pipex: %s: command not found\n", first_param,
			&params);
	execve(path, (char **) params.elements, envp);
	free(path);
	exit_with_error("pipex: failed to execute command\n", NULL, &params);
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
	i = is_here_doc + 1;
	while (++i < argc - 1)
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
	}
	if (output(argv[argc - 1], fd, is_here_doc))
		return (ft_fdprintf(2, "pipex: error writing to outfile\n"), 2);
	while (i-- - is_here_doc - 2)
		waitpid(0, NULL, 0);
	close(fd);
	return (0);
}
