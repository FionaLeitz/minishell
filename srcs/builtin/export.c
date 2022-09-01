/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 14:18:53 by fleitz            #+#    #+#             */
/*   Updated: 2022/08/15 14:45:58 by masamoil         ###   ########.fr       */
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
		return (NULL);
	if (env[0] == NULL)
		return (export);
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

// concat if += and exist, or replace if exist
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
	if (ft_strncmp(arg, tmp->name, limit) == 0 && tmp->name[limit + 1] == '\0')
	{
		free(tmp->value);
		tmp->value = ft_strdup(&arg[limit]);
		return (1);
	}
	return (0);
}

// create if needed
static void	new_export(char *arg, t_params *params)
{
	t_export	*tmp;
	t_export	*tmp2;
	int			limit;

	limit = 0;
	while (arg[limit] != '\0' && arg[limit] != '=')
		limit++;
	tmp = params->export;
	tmp2 = NULL;
	while (tmp)
	{
		if (in_new_export(arg, tmp, limit) == 1)
			return ;
		else if (ft_strncmp(arg, tmp->name, limit) < 0)
		{
			place_new(arg, tmp, tmp2, params);
			return ;
		}
		tmp2 = tmp;
		tmp = tmp->next;
	}
	tmp2->next = new_element(arg);
	return ;
}

// check if arguments
int	ft_export(char **arg, t_params *params)
{
	int	count;
	int	ret;

	ret = 0;
	if (arg[1] == NULL)
	{
		print_export(params);
		return (ret);
	}
	count = 0;
	while (arg[++count])
	{
		if (arg[count][0] == '=' || arg[count][0] == '-'
			|| arg[count][0] == '\0')
		{
			printf("minishell: export: `%s': not a valid identifier\n",
				arg[count]);
			ret = 1;
		}
		else
		{
			new_env(arg[count], params);
			new_export(arg[count], params);
		}
	}
	return (ret);
}
