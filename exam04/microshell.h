/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 16:50:34 by arthur            #+#    #+#             */
/*   Updated: 2022/01/17 19:31:44 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MICROSHELL_H
# define MICROSHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <stdbool.h>
# include <sys/wait.h>

# define	SEMICOLON	0
# define	PIPE		1
# define	WORD		2


typedef struct		s_list
{
	char			*word;
	int				type;
	struct s_list	*next;
}					t_list;


typedef struct	s_data
{
	size_t		nb_cmd;
	t_list		*cmds;
}				t_data;

void	ft_putchar_fd(int fd, char c);
void	ft_putstr(char *s);
void	ft_putstr_err(char *s);
size_t	ft_strlen(char *s);
char	**build_argv(t_data *data);
void	del_next_pipe(t_data **data);
void	free_data(t_data **data);
char	*ft_strdup(char *s);

#endif