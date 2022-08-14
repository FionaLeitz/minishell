/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 18:06:01 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/07 14:41:02 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	exit_status = 0;

// main
int	main(int ac, char **av, char **envp)
{
	t_data		data;
	t_params	params;

	(void)av;
	//tests whether fd is a open fd reffering to a terminal, in this case returns 1
	if (isatty(0) == 0 || isatty(1) == 0)
		return (0);
	params.env = ft_get_env(envp);
	params.export = create_export(params.env);
	//if (ac != 1)
	//	error message ?
	if (ac == 1)
		print_prompt(&data, &params);
	free_export(params.export);
	free_table(params.env);
	return (EXIT_SUCCESS);
}
