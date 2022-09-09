/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 16:18:00 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/03 16:18:18 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// free export
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

// free in exit, when exit
int	free_exit(t_params *params, t_data *data, int *old_fd)
{
	if (old_fd != NULL)
	{
		close(old_fd[0]);
		close(old_fd[1]);
	}
	free(data->pid);
	free(data->pipe_fd);
	free_struct(data);
	free_params(params);
	return (12);
}

// free data
void	free_struct(t_data *data)
{
	t_token	*tmp;

	if (data->input != NULL)
		free(data->input);
	if (data->trimmed != NULL)
		free(data->trimmed);
	while (data->head != NULL)
	{
		tmp = data->head;
		if (tmp->fds[0] > 0)
			close(tmp->fds[0]);
		if (tmp->fds[1] != 1 && tmp->fds[1] >= 0)
			close(tmp->fds[1]);
		free(tmp->value);
		free_table(tmp->args);
		free_table(tmp->red);
		data->head = data->head->next;
		free(tmp);
	}
	init_data(data);
}

// free params
void	free_params(t_params *params)
{
	t_export	*tmp;

	free_table(params->env);
	while (params->export)
	{
		tmp = params->export;
		free(tmp->name);
		free(tmp->value);
		params->export = params->export->next;
		free(tmp);
	}
}

// free all if error with command in select builtin
void	free_command_no(t_params *params, int *old_fd)
{
	t_token	*tmp;

	tmp = params->data->head;
	while (tmp)
	{
		if (tmp->fds[0] > 0)
			close(tmp->fds[0]);
		if (tmp->fds[1] != 1 && tmp->fds[1] >= 0)
			close(tmp->fds[1]);
		tmp = tmp->next;
	}
	free(params->data->pid);
	free(params->data->pipe_fd);
	free_struct(params->data);
	free_params(params);
	if (old_fd != NULL)
	{
		close(old_fd[0]);
		close(old_fd[1]);
	}
	exit(g_exit_st);
}
