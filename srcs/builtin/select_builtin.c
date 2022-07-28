/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 09:30:11 by fleitz            #+#    #+#             */
/*   Updated: 2022/05/10 09:30:12 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// get path of command if not built-in
int	get_path(char **arg, t_params *params)
{
	int		i;
	char	**path;
	char	*tmp;

	if (access(arg[0], F_OK | X_OK) != -1)
		return (0);
	i = 0;
	while (params->env[i] && ft_strncmp(params->env[i], "PATH=", 5) != 0)
		i++;
	if (params->env[i] == NULL)
		return (1);
	path = ft_split(&params->env[i][5], ':');
	if (path == NULL)
		return (1);
	i = -1;
	while (path[++i])
	{
		tmp = ft_strcat_malloc(path[i], "/");
		if (tmp == NULL)
		{
			free_table(path);
			return (1);
		}
		free(path[i]);
		path[i] = ft_strcat_malloc(tmp, arg[0]);
		free(tmp);
		if (path[i] == NULL)
		{
			free_table(path);
			free_table(&path[i + 1]);
			return (1);
		}
		if (access(path[i], F_OK | X_OK) != -1)
		{
			free(arg[0]);
			arg[0] = ft_strdup(path[i]);
			free_table(path);
			break ;
		}
	}
	return (0);
}

// select if built-in or execve
void	ft_select_builtin(t_token *token, t_params *params)
{
	if (params->env == NULL)
		ft_printf("Error malloc...\n");
	if (ft_strncmp(token->args[0], "cd", 3) == 0)
		ft_cd(token->args, params);
	else if (ft_strncmp(token->args[0], "echo", 5) == 0)
		ft_echo(token->args);
	else if (ft_strncmp(token->args[0], "env", 4) == 0)
		ft_env(token->args, params);
	else if (ft_strncmp(token->args[0], "pwd", 4) == 0)
		ft_pwd(token->args);
	else if (ft_strncmp(token->args[0], "exit", 5) == 0)
		ft_exit(token->args);
	else if (ft_strncmp(token->args[0], "export", 7) == 0)
		ft_export(token->args, params);
	else if (ft_strncmp(token->args[0], "unset", 6) == 0)
		ft_unset(token->args, params);
	else
	{
		get_path(token->args, params);
		execve(token->args[0], token->args, params->env);
		write(2, "minishell: ", 11);
		write(2, token->args[0], ft_strlen(token->args[0]));
		write(2, " : command not found\n", 21);
	}
	return ;
}
