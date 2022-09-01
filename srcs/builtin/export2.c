/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 16:12:28 by fleitz            #+#    #+#             */
/*   Updated: 2022/06/13 16:12:30 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// if new element needs to be placed
void	place_new(char *arg, t_export *new, t_export *new2, t_params *params)
{
	if (new2 == NULL)
	{
		params->export = new_element(arg);
		params->export->next = new;
		return ;
	}
	new2->next = new_element(arg);
	new2->next->next = new;
	return ;
}

// free export
void	*free_export(t_export *export)
{
	t_export	*tmp;

	while (export)
	{
		if (export->name)
			free(export->name);
		if (export->value)
			free(export->value);
		tmp = export;
		export = export->next;
		free(tmp);
	}
	return (NULL);
}

// create empty string if no env
static int	empty_str(char *str, t_export *element)
{
	int	count;

	count = 0;
	if (str == NULL)
	{
		element->name = strdup("\0");
		element->value = strdup("\0");
		return (-1);
	}
	while (str[count] && str[count] != '=')
		count++;
	return (count);
}

// create new t_export element
t_export	*new_element(char *str)
{
	t_export	*element;
	int			count;

	element = malloc(sizeof(t_export));
	if (element == NULL)
		return (NULL);
	ft_bzero(element, sizeof(t_export));
	count = empty_str(str, element);
	if (count == -1)
		return (element);
	element->name = ft_strndup(str, count);
	if (element->name == NULL)
	{
		free(element);
		return (NULL);
	}
	if (element->name[count - 1] == '+')
		element->name[count - 1] = '\0';
	element->value = ft_strdup(&str[count]);
	if (element->value == NULL)
	{
		free_export(element);
		return (NULL);
	}
	return (element);
}

// print export if no argument
void	print_export(t_params *params)
{
	t_export	*tmp;

	tmp = params->export;
	while (params->export)
	{
		if (params->export->name[0] != '\0')
		{
			ft_printf("declare -x %s", params->export->name);
//		if (params->export->value[0] != '\0')
			ft_printf("=\"%s\"", &params->export->value[1]);
		}
		if (params->export->name[0] != '\0')
			ft_printf("\n");
		params->export = params->export->next;
	}
	params->export = tmp;
}
