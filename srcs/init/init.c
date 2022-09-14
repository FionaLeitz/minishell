/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 13:35:21 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/15 16:34:22 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// put every variable in data to NULL
void	init_data(t_data *data)
{
	data->input = NULL;
	data->trimmed = NULL;
	data->head = NULL;
	data->pid = NULL;
	data->pipe_fd = NULL;
	data->i = 0;
}

// put every variable in token to NULL
void	init_token(t_token *token)
{
	token->value = NULL;
	token->args = NULL;
	token->red = NULL;
	token->next = NULL;
	token->prev = NULL;
	token->fds[0] = 0;
	token->fds[1] = 1;
}

// set up error malloc
static int	wrong_malloc(t_params params)
{
	free_params(&params);
	return (errno);
}

// start params
int	initiate_params(t_params *params, char **envp)
{
	if (envp != NULL)
	{
		params->env = ft_get_env(envp);
		if (errno == 12)
			return (12);
		if (params->env[0] != NULL)
			ft_shlvl(params->env);
		if (errno == 12)
			return (wrong_malloc(*params));
		params->export = create_export(params->env);
		if (errno == 12)
			return (wrong_malloc(*params));
	}
	return (0);
}

//increments SHLVL variable
int	ft_shlvl(char **envp)
{
	char	shlvl[12];
	char	*tmp;

	while (*envp && ft_strncmp("SHLVL=", *envp, 6))
		envp++;
	if (!*envp)
		return (-1);
	ft_itoa_no_malloc(ft_atoi(*envp + 6) + 1, shlvl);
	tmp = ft_strndup(*envp, 6);
	if (tmp == NULL)
		return (set_error_malloc("creating env\n"));
	free(*envp);
	*envp = ft_strjoin(tmp, shlvl);
	free(tmp);
	if (*envp == NULL)
		return (set_error_malloc("creating env\n"));
	return (0);
}
