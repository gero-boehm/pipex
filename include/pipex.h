/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:44:25 by gbohm             #+#    #+#             */
/*   Updated: 2023/04/23 13:10:07 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define STDIN 0
# define STDOUT 1

# include <stddef.h>

int	split_params(char *cmd, char ***params);
int	is_quote(char c);
int	get_command_path(char *cmd, char *const *envp, char **path);

int	run(int fd, int is_here_doc, char **argv, char *const *envp);

int	get_infile(char *fspath, int *fd);
int	get_write_fd(char *fspath, int append, int *fd);
int	output(char *file, int fd, int is_here_doc);

#endif
