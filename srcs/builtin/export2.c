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

t_export	*new_element(char *str)
{
	t_export	*element;
	int			count;

	element = malloc(sizeof(t_export));
	if (element == NULL)
		return (NULL);
	ft_bzero(element, sizeof(t_export));
	count = 0;
	if (str == NULL)
	{
		element->name = strdup("\0");
		element->value = strdup("\0");
		return (element);
	}
	while (str[count] && str[count] != '=')
		count++;
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

void	print_export(t_export *export)
{
	t_export	*tmp;

	tmp = export;
	while (export)
	{
		ft_printf("%s", export->name);
		if (export->value[0] != '\0')
			ft_printf("=\"%s\"", &export->value[1]);
		if (export->name[0] != '\0')
			ft_printf("\n");
		export = export->next;
	}
	export = tmp;
}
