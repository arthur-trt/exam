/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 16:49:44 by arthur            #+#    #+#             */
/*   Updated: 2022/01/17 20:12:39 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

void	ft_putchar_fd(int fd, char c)
{
	write(fd, &c, 1);
}

void	ft_putstr(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		ft_putchar_fd(STDIN_FILENO, s[i++]);
}

void	ft_putstr_err(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		ft_putchar_fd(STDERR_FILENO, s[i++]);
}

size_t	ft_strlen(char *s)
{
	size_t	len;

	len = 0;
	while(s[len] != '\0')
		len++;
	return (len);
}

char	*ft_strdup(char *s)
{
	size_t	i;
	size_t	len;
	char	*res;

	len = ft_strlen(s);
	res = malloc((len + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (i <= len)
	{
		res[i] = s[i];
		i++;
	}
	return (res);
}

size_t	count_args(t_data *data)
{
	size_t	count;
	t_list	*tmp;

	tmp = data->cmds;
	count = 0;
	while (tmp && tmp->type != PIPE)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

char	**build_argv(t_data *data)
{
	size_t	i;
	char	**argv;
	t_list	*tmp;

	argv = (char **)malloc((count_args(data) + 1) * sizeof(char *));
	tmp = data->cmds;
	i = 0;
	while (tmp && tmp->type != PIPE)
	{
		argv[i] = ft_strdup(tmp->word);
		i++;
		tmp = tmp->next;
	}
	argv[i] = NULL;
	return (argv);
}

void	del_next_pipe(t_data **data)
{
	t_list	*next;
	t_list	*tmp;

	tmp = (*data)->cmds;
	while (tmp && tmp->type != PIPE)
	{
		next = tmp->next;
		free(tmp->word);
		tmp->word = NULL;
		free(tmp);
		tmp = NULL;
		(*data)->cmds = next;
		tmp = next;
	}
	if (tmp && tmp->type == PIPE)
	{
		next = tmp->next;
		free(tmp->word);
		tmp->word = NULL;
		free(tmp);
		tmp = NULL;
		(*data)->cmds = next;
		tmp = next;
	}
}