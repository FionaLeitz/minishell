/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fct_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 18:19:53 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/11 16:56:10 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// create new token element
static t_token	*new_node(char *value)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
	{
		set_error_malloc("parse\n");
		return (NULL);
	}
	init_token(new);
	new->value = value;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

// create new element and place in last position on the list
int	push_back(t_data *data, char *value)
{
	t_token	*new;
	t_token	*tmp;

	new = new_node(value);
	if (new == NULL)
		return (-1);
	tmp = data->head;
	if (data->head == NULL)
	{
		data->head = new;
		return (0);
	}
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
	return (0);
}

// get the number of element
int	ft_size(t_token *lst)
{
	int	i;

	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
