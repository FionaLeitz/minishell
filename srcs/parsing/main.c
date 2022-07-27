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

int	main(int ac, char **av, char **envp)
{
	t_data		data;
	t_params	params;

	(void)av;
	params.env = ft_get_env(envp);
	printf("Tout va bien 1\n");
	params.export = create_export(params.env);
	printf("Tout va bien 2\n");
	init_data(&data);
	if (ac == 1)
		print_prompt(&data, &params);
	free_export(params.export);
	free_table(params.env);
	return (EXIT_SUCCESS);
}
