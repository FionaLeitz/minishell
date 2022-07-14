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

t_token	*new_node(char *value)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = value;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	push_front(t_data *data, char *value)
{	
	t_token	*new;

	new = new_node(value);
	if (data->head == NULL)
	{
		data->head = new;
		return ;
	}
	data->head->prev = new;
	new->next = data->head;
	data->head = new;
}

void	push_back(t_data *data, char *value)
{
	t_token	*new;
	t_token	*tmp;

	new = new_node(value);
	tmp = data->head;
	if (data->head == NULL)
	{
		data->head = new;
		return ;
	}
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

void	preview(t_data *data)
{
	t_token	*tmp;

	tmp = data->head;
	if (tmp == NULL)
		return ;
	while (tmp != NULL)
	{
		printf("str value: %s\n", tmp->value);
		tmp = tmp->next;
	}
}

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
