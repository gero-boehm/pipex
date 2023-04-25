/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:44:18 by gbohm             #+#    #+#             */
/*   Updated: 2023/04/25 14:00:01 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "pipex.h"
#include "ft_printf.h"

int	main(int argc, char **argv, char *const *envp)
{
	int	infile;

	if (argc != 5)
	{
		ft_fdprintf(2, "pipex: incorrect argument count\n");
		exit (1);
	}
	get_infile(argv[1], &infile);
	if (envp == NULL)
		return (2);
	run(infile, 0, argv, envp);
	// system("leaks pipex");
	return (0);
}
