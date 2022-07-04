/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 14:18:53 by fleitz            #+#    #+#             */
/*   Updated: 2022/06/03 14:18:56 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

t_export	*create_export(char **env)
{
	t_export	*export;
	t_export	*tmp;
	int			count;

	if (env == NULL)
		return (NULL);
	export = new_element(env[0]);
	if (export == NULL)
		return (NULL);
	tmp = export;
	count = 0;
	while (env[++count])
	{
		export->next = new_element(env[count]);
		if (export->next == NULL)
			return (free_export(tmp));
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

static int	in_new_export(char *arg, t_export *tmp, int limit)
{
	char	*str;

	if (ft_strncmp(arg, tmp->name, limit - 1) == 0
		&& tmp->name[limit - 1] == '\0' && arg[limit - 1] == '+')
	{
		str = malloc(sizeof(char) * (ft_strlen(&arg[limit])
					+ ft_strlen(tmp->value)));
		str[0] = '\0';
		ft_strcat(str, tmp->value);
		ft_strcat(str, &arg[limit + 1]);
		free(tmp->value);
		tmp->value = str;
		return (1);
	}
	if (ft_strncmp(arg, tmp->name, limit) == 0 && tmp->name[limit] == '\0')
	{
		free(tmp->value);
		tmp->value = ft_strdup(&arg[limit]);
		return (1);
	}
	return (0);
}

static void	new_export(char *arg, t_export *export)
{
	t_export	*tmp;
	t_export	*tmp2;
	t_export	*tmp3;
	int			limit;

	limit = 0;
	while (arg[limit] != '\0' && arg[limit] != '=')
		limit++;
	tmp = export;
	while (tmp)
	{
		if (in_new_export(arg, tmp, limit) == 1)
			return ;
		else if (ft_strncmp(arg, tmp->name, limit) < 0)
		{
			tmp2 = tmp->next;
			tmp->next = new_element(arg);
			tmp->next->next = tmp2;
			return ;
		}
		tmp3 = tmp;
		tmp = tmp->next;
	}
	tmp3->next = new_element(arg);
}

char	**ft_export(char **arg, char **env, t_export *export)
{
	int		count;

	if (arg[1] == NULL)
	{
		print_export(export);
		return (env);
	}
	count = 0;
	while (arg[++count])
	{
		env = new_env(arg[count], env);
		new_export(arg[count], export);
	}
	return (env);
}
