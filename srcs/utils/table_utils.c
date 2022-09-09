/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 09:30:18 by fleitz            #+#    #+#             */
/*   Updated: 2022/05/10 09:30:20 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Print string table
void	print_table(char **table)
{
	int	i;
	int	j;

	i = 0;
	while (table[i])
	{
		j = -1;
		while (table[i][++j])
		{
			if (table[i][j] == -1)
				ft_printf("\'");
			else if (table[i][j] == -2)
				ft_printf("\"");
			else
				ft_printf("%c", table[i][j]);
		}
		if (table[i][0] != '\0')
			ft_printf("\n");
		i++;
	}
}

// free string table
void	free_table(char **table)
{
	int	i;

	i = 0;
	if (table == NULL)
		return ;
	while (table[i])
	{
		free(table[i]);
		i++;
	}
	free(table);
}
