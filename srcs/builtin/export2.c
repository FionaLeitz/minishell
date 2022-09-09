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
