/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 14:41:15 by fleitz            #+#    #+#             */
/*   Updated: 2022/05/11 14:41:18 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_unset_export(char **arg, t_export *export)
{
	int			i;
	int			j;
	t_export	*tmp;

	i = 1;
	while (arg[i])
	{
		tmp = export;
		while (tmp)
		{
			j = 0;
			while (arg[i][j] == tmp->name[j] && arg[i][j] != '\0')
				j++;
			if (arg[i][j] == '\0' && tmp->name[j] == '\0')
			{
				tmp->name[0] = '\0';
				tmp->value[0] = '\0';
				break ;
			}
			tmp = tmp->next;
		}
		i++;
	}
}

int	ft_unset(char **arg, char **env, t_export *export)
{
	int			i;
	int			j;
	int			count;

	i = 0;
	count = 0;
	while (arg[++i])
	{
		count = -1;
		while (env[++count])
		{
			j = -1;
			while (arg[i][++j])
				if (arg[i][j] != env[count][j])
					break ;
			if (arg[i][j] == '\0' && env[count][j] == '=')
			{
				env[count][0] = '\0';
				break ;
			}
		}
	}
	ft_unset_export(arg, export);
	return (0);
}
