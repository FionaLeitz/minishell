/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 15:58:23 by masamoil          #+#    #+#             */
/*   Updated: 2022/09/09 15:58:29 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// bubble sort with strcmp
static void	bubble_sort_export(t_export *export)
{
	char		*str;
	t_export	*tmp;

	tmp = export->next;
	while (tmp)
	{
		if (ft_strcmp(export->name, tmp->name) > 0)
		{
			str = export->name;
			export->name = tmp->name;
			tmp->name = str;
			str = export->value;
			export->value = tmp->value;
			tmp->value = str;
		}
		tmp = tmp->next;
	}
}

// create export from env at the beggining of minishell
t_export	*create_export(char **env)
{
	t_export	*export;
	t_export	*tmp;
	int			count;

	export = new_element(env[0]);
	if (export == NULL)
		return (error_malloc_return("export\n", NULL));
	if (env[0] == NULL)
		return (export);
	tmp = export;
	count = 0;
	while (env[++count])
	{
		export->next = new_element(env[count]);
		if (export->next == NULL)
			return (error_malloc_return("export\n", NULL));
		export = export->next;
	}
	export = tmp;
	while (export)
	{
		bubble_sort_export(export);
		export = export->next;
	}
	return (tmp);
}
