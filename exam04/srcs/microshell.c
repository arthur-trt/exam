/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atrouill <atrouill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 16:46:51 by arthur            #+#    #+#             */
/*   Updated: 2022/02/16 11:14:36 by atrouill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

void	fatal_error(t_data **data)
{
	ft_putstr_err("error: fatal\n");
	while ((*data)->cmds != NULL)
		del_next_pipe(data);
	free(*data);
	(*data) = NULL;
	exit (EXIT_FAILURE);
}

void	add_to_list(t_data **data, t_list **cmds, char *word, int type)
{
	t_list	*new;
	t_list	*tmp;

	new = malloc(sizeof(t_list));
	if (new == NULL)
		fatal_error(data);
	new->type = type;
	new->word = ft_strdup(word);
	if (new->word == NULL)
		fatal_error(data);
	new->next = NULL;
	if ((*cmds) == NULL)
	{
		(*cmds) = new;
	}
	else
	{
		tmp = (*cmds);
		while (tmp->next != NULL)
			tmp = tmp->next;
		if (tmp)
			tmp->next = new;
	}
}

void	parse_args(t_data **data, char **argv, int *i)
{
	while (argv[(*i)] != NULL && strcmp(argv[(*i)], ";") != 0)
	{
		if (strcmp(argv[(*i)], "|") == 0)
		{
			(*data)->nb_cmd += 1;
			add_to_list(data, &((*data)->cmds), "|", PIPE);
		}
		else
			add_to_list(data, &((*data)->cmds), argv[(*i)], WORD);
		(*i)++;
	}
}

void	exec_simple(t_data *data, char **envp)
{
	char 	**argv;
	pid_t	pid;
	int		ret;

	if (strcmp("cd", data->cmds->word) == 0)
	{
		ft_cd(data);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			argv = build_argv(data);
			if (execve(argv[0], argv, envp) != 0)
			{
				ft_putstr_err("error: cannot execute ");
				ft_putstr_err(argv[0]);
				ft_putchar_fd(2, '\n');
			}
			exit(0);
		}
		waitpid(pid, &ret, 0);
	}
}

void	piper(t_data *data, char **envp)
{
	int		fds[4];
	pid_t	pid;
	size_t		i;
	size_t	nb_exec;
	int		ret;

	pipe(fds);
	pipe(fds + 2);
	nb_exec = 1;
	while(data->cmds != NULL)
	{
		pid = fork();
		if (pid == 0)
		{
			if (nb_exec != 1)
				dup2(fds[0], STDIN_FILENO);
			if (nb_exec != data->nb_cmd)
				dup2(fds[3], STDOUT_FILENO);
			i = 0;
			while (i < 4)
				close(fds[i++]);
			exec_simple(data, envp);
			exit(0);
		}
		close(fds[0]);
		close(fds[1]);
		fds[0] = fds[2];
		fds[1] = fds[3];
		pipe(fds + 2);
		nb_exec++;
		del_next_pipe(&data);
	}
	i = 0;
	while (i < nb_exec)
	{
		waitpid(-1, &ret, 0);
		i++;
	}
	i = 0;
	while (i < 4)
		close(fds[i++]);

}

void		exec(t_data *data, char**envp)
{
	int		fd[2];

	fd[0] = dup(STDIN_FILENO);
	fd[1] = dup(STDOUT_FILENO);
	if (data->nb_cmd > 1)
	{
		piper(data, envp);
	}
	else
	{
		exec_simple(data, envp);
	}
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;
	int		i;

	i = 1;
	while(argc > i)
	{
		data = malloc(sizeof(t_data));
		if (data == NULL)
			fatal_error(&data);
		data->cmds = NULL;
		data->nb_cmd = 1;
		parse_args(&data, argv, &i);
		if (data->cmds != NULL)
			exec(data, envp);
		del_next_pipe(&data);
		free(data);
		(data) = NULL;
		i++;
	}
	return (0);
}
