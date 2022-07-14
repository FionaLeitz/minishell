/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 13:43:02 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/08 15:04:58 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token	*l_redirect(t_data *data, t_token *token_list)
{
	if (ft_strncmp(data->trimmed + data->i, "<<", 2) != 0)
		printf("euhmmm");
	else
		push_back(data, "<");
	return (token_list);
}
