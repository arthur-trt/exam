/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 16:46:51 by arthur            #+#    #+#             */
/*   Updated: 2022/01/17 19:45:29 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

void	add_to_list(t_list **cmds, char *word, int type)
{
	t_list	*new;
	t_list	*tmp;
	
	new = malloc(sizeof(t_list));
	new->type = type;
	new->word = ft_strdup(word);
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
			add_to_list(&((*data)->cmds), "|", PIPE);
		}
		else
			add_to_list(&((*data)->cmds), argv[(*i)], WORD);
		(*i)++;
	}
}

void	exec_simple(t_data *data, char **envp)
{
	char 	**argv;
	pid_t	pid;
	int		ret;

	pid = fork();
	if (pid == 0)
	{
		argv = build_argv(data);
		execve(argv[0], argv, envp);
		exit(0);
	}
	waitpid(pid, &ret, 0);
}

void	piper(t_data *data, char **envp)
{
	int		fds[4];
	pid_t	pid;
	int		i;
	int		nb_exec;
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

int		exec(t_data *data, char**envp)
{
	char	**argv;
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
		data->cmds = NULL;
		data->nb_cmd = 1;
		parse_args(&data, argv, &i);
		exec(data, envp);
		del_next_pipe(&data);
		free(data);
		(data) = NULL;
		i++;
	}
	return (0);
}