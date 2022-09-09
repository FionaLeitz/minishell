/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 16:02:52 by masamoil          #+#    #+#             */
/*   Updated: 2022/09/09 16:02:56 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// print export if no argument
int	print_export(t_params *params)
{
	t_export	*tmp;
	int			i;

	tmp = params->export;
	while (params->export)
	{
		if (params->export->name[0] != '\0')
		{
			ft_printf("declare -x %s=\"", params->export->name);
			i = 0;
			while (params->export->value[i] && params->export->value[++i])
			{
				if (params->export->value[i] == -1)
					ft_printf("\'");
				else if (params->export->value[i] == -2)
					ft_printf("\"");
				else
					ft_printf("%c", params->export->value[i]);
			}
			ft_printf("\"\n");
		}
		params->export = params->export->next;
	}
	params->export = tmp;
	return (0);
}
