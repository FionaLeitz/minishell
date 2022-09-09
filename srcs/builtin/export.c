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

// if new element needs to be placed
int	place_new(char *arg, t_export *new, t_export *new2, t_params *params)
{
	if (new2 == NULL)
	{
		params->export = new_element(arg);
		if (params->export == NULL)
			return (set_error_malloc("export\n"));
		params->export->next = new;
		return (0);
	}
	new2->next = new_element(arg);
	if (new2->next == NULL)
		return (set_error_malloc("export\n"));
	new2->next->next = new;
	return (0);
}

// concat if += and exist, or replace if exist
static int	in_new_export(char *arg, t_export *tmp, int limit)
{
	char	*str;

	if (ft_strncmp(arg, tmp->name, limit - 1) == 0
		&& tmp->name[limit - 1] == '\0' && arg[limit - 1] == '+')
	{
		str = ft_strcat_malloc(tmp->value, &arg[limit + 1]);
		if (str == NULL)
			return (set_error_malloc("export\n"));
		free(tmp->value);
		tmp->value = str;
		return (0);
	}
	if ((int)ft_strlen(tmp->name) >= limit && ft_strncmp(arg, tmp->name,
			limit) == 0 && tmp->name[limit] == '\0')
	{
		free(tmp->value);
		tmp->value = ft_strdup(&arg[limit]);
		if (tmp->value == NULL)
			return (set_error_malloc("export\n"));
		return (0);
	}
	return (1);
}	

// create if needed
static int	new_export(char *arg, t_params *params)
{
	t_export	*tmp;
	t_export	*tmp2;
	int			limit;
	int			ret;

	limit = 0;
	while (arg[limit] != '\0' && arg[limit] != '=')
		limit++;
	tmp = params->export;
	tmp2 = NULL;
	while (tmp)
	{
		ret = in_new_export(arg, tmp, limit);
		if (ret == -1 || ret == 0)
			return (ret);
		else if (ft_strncmp(arg, tmp->name, limit) < 0)
			return (place_new(arg, tmp, tmp2, params));
		tmp2 = tmp;
		tmp = tmp->next;
	}
	tmp2->next = new_element(arg);
	if (tmp2->next == NULL)
		return (set_error_malloc("export\n"));
	return (0);
}

// check if arg is invalid
static int	ft_invalid(char *str)
{
	int	i;

	if (str[0] == '=' || str[0] == '-' || str[0] == '\0')
		return (-1);
	i = -1;
	while (str[++i] && str[i] != '=')
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (-1);
	}
	return (0);
}

// check if arguments
int	ft_export(char **arg, t_params *params)
{
	int	count;
	int	ret;

	ret = 0;
	if (arg[1] == NULL)
		return (print_export(params));
	count = 0;
	while (arg[++count])
	{
		if (ft_invalid(arg[count]) == -1)
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(arg[count], 2);
			ft_putstr_fd("\': not a valid identifier\n", 2);
			ret = 1;
		}
		else
		{
			if (new_env(arg[count], params) == -1)
				return (-1);
			if (new_export(arg[count], params) == -1)
				return (-1);
		}
	}
	return (ret);
}
