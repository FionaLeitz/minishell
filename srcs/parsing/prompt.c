/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:13:34 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/25 13:58:47 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// void	print_table2(char **table, int j)
// {
// 	int	i;

// 	i = 0;
// 	if (j == 2)
// 		printf("red :\n");
// 	if (j == 1)
// 		printf("args :\n");
// 	while (table[i])
// 	{
// 		if (table[i] == NULL)
// 			printf("C'est vide\n");
// 		printf("%s", table[i]);
// 		printf("\n");
// 		i++;
// 	}
// 	printf("------\n");
// }

int	syntax_check(t_data *data)
{
	data->trimmed = ft_strtrim(data->input, " \t\n\v\f\r");
	if (data->trimmed[0] == '|')
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		free_struct(data);
		return (-1);
	}
	if (data->input != NULL && ft_strlen(data->input) != 0)
		add_history(data->input);
	//rl_clear_history();
	if (check_string(data) == -1)
	{
		free_struct(data);
		return (-1);
	}
	if (check_quotes(data) == -1)
	{
		printf("minishell: quotes are unclosed\n");
		free_struct(data);
		return (-1);
	}
	return (0);
}

// char	*replace(char **env, char *str, int size)
// {
// 	int	count;

// 	if (size == 0)
// 		return (NULL);
// 	count = -1;
// 	while (env[++count])
// 	{
// 		if (ft_strncmp(env[count], str, size) == 0 && env[count][size] == '=')
// 			return (&env[count][size + 1]);
// 	}
// 	return (NULL);
// }

// int	replace_var(t_token *token, t_data *data, t_params *params)
// {
// 	int		save;
// 	char	*str;
// 	char	*tmp;

// 	(void)params;
// 	int	i = 1;
// 	str = NULL;
// 	while (token)
// 	{
// 		data->i = 0;
// 		while (token->value[data->i])
// 		{
// 			if (token->value[data->i] == '\'')
// 				jump_quotes(token->value, data);
// 			if (token->value[data->i] == '$')
// 			{
// 				save = data->i++;
// 				while (token->value[data->i] && ft_check_whitespace(token->value[data->i]) != 0
// 					&& token->value[data->i] != '\'' && token->value[data->i] != '\"' && token->value[data->i] != '$')
// 					data->i++;
// 				str = replace(params->env, &token->value[save + 1], data->i - save - 1);
// 				if (str == NULL)
// 				{
// 					ft_memmove(&token->value[save], &token->value[data->i], ft_strlen(&token->value[data->i]) + 1);
// 					data->i = save;
// 				}
// 				else
// 				{
// 					tmp = malloc(sizeof(char) * (save + ft_strlen(str) + ft_strlen(&token->value[data->i]) + 1));
// 					if (tmp == NULL)
// 						return (-1);
// 					ft_bzero(tmp, save + ft_strlen(str) + ft_strlen(&token->value[data->i]) + 1);
// 					tmp = ft_memmove(tmp, token->value, save);
// 					tmp = ft_strcat(tmp, str);
// 					tmp = ft_strcat(tmp, &token->value[data->i]);
// 					free(token->value);
// 					token->value = tmp;
// 					data->i = save + ft_strlen(str) - 1;
// 				}
// 				i++;
// 			}
// 			data->i++;
// 		}
// 		printf("After replace : %s\n", token->value);
// 		token = token->next;
// 	}
// 	return (0);
// }

void	ft_cut(t_data *data, t_params *params)
{
	t_token	*tmp;
	char	*save;
	(void)params;


	first_pipe_cut(data);
	tmp = data->head;
	while (tmp)
	{
		save = ft_strtrim(tmp->value, " \t\n\v\f\r");
		free(tmp->value);
		tmp->value = save;
		tmp = tmp->next;
	}

	tmp = data->head;
	while (tmp)
	{
		count_red(data, tmp);
		tmp = tmp->next;
	}
	// tmp = data->head;
	// if (replace_var(tmp, data, params) == -1)
	// {
	// 	printf("ERROR REPLACE_VAR\n");
	// 	return ;
	// }
	tmp = data->head;
	while (tmp)
	{
		create_tab(data, tmp);
		tmp = tmp->next;
	}
	tmp = data->head;
	del_quotes(tmp);
	tmp = data->head;
	while (tmp)
	{
		red_tab(data, tmp);
		tmp = tmp->next;
	}
}

int	print_prompt(t_data *data, t_params *params)
{
	t_token	*tmp;

	tmp = NULL;
	while (1)
	{
		init_data(data);
		//init_token(tmp);
		signal(SIGINT, sig_manage);
		signal(SIGQUIT, sig_manage);
		data->input = readline(PROMPT);
		if (!data->input)
			ft_exit_d(data);
		if (syntax_check(data) == 0)
			ft_cut(data, params);
		tmp = data->head;
		while (tmp && tmp->args[0])
		{
			printf("cmd :\n");
			print_table(tmp->args);
			printf("----------\n");
			printf("red: \n");
			print_table(tmp->red);
			printf("----------\n");
			printf("red_tab: \n");
			print_table(tmp->red_tab);
			printf("----------\n");	
		//	params->env = ft_select_builtin(tmp, params);
			tmp = tmp->next;
		}
		free_struct(data);
	}
	rl_clear_history();
	return (0);
}
