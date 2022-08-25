/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 18:06:01 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/25 10:41:02 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	g_exit_st = 0;

// main
int	main(int ac, char **av, char **envp)
{
	t_data		data;
	t_params	params;

	(void)av;
	//tests whether fd is an open fd referring to a terminal,
	//in this case returns 1, otherwise => 0 end errno's set
	if (isatty(0) == 0 || isatty(1) == 0)
		return (0);
	params.env = ft_get_env(envp);
	ft_shlvl(params.env);
	params.export = create_export(params.env);
	if (ac != 1)
		ft_putstr_fd("This minishell does not take arguments\n", 2);
	if (ac == 1)
		print_prompt(&data, &params);
	free_export(params.export);
	free_table(params.env);
	return (g_exit_st);
}
