/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   only_heredocs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 08:38:33 by fleitz            #+#    #+#             */
/*   Updated: 2022/09/12 08:38:46 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// if parse error, suppress everything except heredocs before error
static int	modify_line(t_data *data, int nbr)
{
	int	save;

	save = 0;
	while (data->trimmed[data->i] != '\0'
		&& ft_space(data->trimmed[data->i]) == 0)
	{
		data->i++;
		nbr++;
	}
	while (data->trimmed[data->i] != '\0'
		&& ft_space(data->trimmed[data->i]) != 0)
	{
		save = data->i;
		if (data->trimmed[data->i] == '\''
			|| data->trimmed[data->i] == '\"')
			jump_quotes(data->trimmed, data);
		else
			data->i++;
		nbr += data->i - save;
	}
	return (nbr);
}

// if no delimiter after heredoc with parse error
static int	if_str_zero(char *str, int i)
{
	if (str[i] == '\0')
	{
		str[0] = '\0';
		return (-1);
	}
	return (0);
}

// if parse error, keep only heredocs before error
void	only_heredocs(t_data *data)
{
	int	save;
	int	save2;

	g_exit_st = 2;
	data->i -= 1;
	ft_bzero(&data->trimmed[data->i], ft_strlen(&data->trimmed[data->i]));
	data->i = 0;
	save = 0;
	while (data->trimmed[data->i])
	{
		save2 = 2;
		if (data->trimmed[data->i] == '<' && data->trimmed[data->i + 1] == '<')
		{
			ft_memcpy(&data->trimmed[save], &data->trimmed[data->i],
				ft_strlen(&data->trimmed[data->i]) + 2);
			data->i = save + 2;
			if (if_str_zero(data->trimmed, data->i) == -1)
				return ;
			save += modify_line(data, save2);
		}
		data->i++;
	}
	data->trimmed[save] = '\0';
}
