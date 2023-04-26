/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:44:18 by gbohm             #+#    #+#             */
/*   Updated: 2023/04/26 17:08:31 by gbohm            ###   ########.fr       */
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
		return (1);
	}
	if (envp == NULL)
		return (2);
	get_infile(argv[1], &infile);
	run(infile, 0, argv, envp);
	return (0);
}
