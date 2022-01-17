/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 19:58:11 by arthur            #+#    #+#             */
/*   Updated: 2022/01/17 20:04:08 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

void	ft_cd(t_data *data)
{
	if (data->cmds != NULL && data->cmds->next != NULL && data->cmds->next->next == NULL)
	{
		if (chdir(data->cmds->next->word) != 0)
		{
			ft_putstr_err("error: cd: cannot change directory to ");
			ft_putstr_err(data->cmds->next->word);
			ft_putchar_fd(2, '\n');
		}
	}
	else
	{
		ft_putstr_err("error: cd: bad arguments\n");
	}
}