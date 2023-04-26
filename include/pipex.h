/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:44:25 by gbohm             #+#    #+#             */
/*   Updated: 2023/04/26 11:23:13 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define STDIN 0
# define STDOUT 1

# include <stddef.h>
# include <libft.h>

int		is_quote(char c);
int		is_backslash(char c);
int		is_space(char c);

int		split_params(char *cmd, t_array *params);
int		get_command_path(char *cmd, char *const *envp, char **path);

int		run(int fd, int is_here_doc, char **argv, char *const *envp);

int		get_infile(char *fspath, int *fd);
int		get_write_fd(char *fspath, int append, int *fd);
int		output(char *file, int fd, int is_here_doc);

int		split_str_into_tokens(char *str, t_array *tokens);
int		make_str_from_tokens(t_array *tokens, char **str);

int		is_escaped(t_array *tokens, int index);
int		is_first_token_space(t_array *tokens);
int		is_last_token(t_array *tokens, unsigned long index);
int		is_token_unquoted_space(char *token, char quote);
int		is_unescaped_quote(t_array *tokens, unsigned long index);

void	free_str_array(t_array *strs);
int		reduce_backslashes(char **str);
int		toggle_quotes(char *quote, char token);

#endif
