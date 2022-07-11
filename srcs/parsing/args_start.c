#include "../../minishell.h"

int first_pipe_cut(t_data *data)
{
	int	count;
	char	quote;

	data->i = 0;
	count = 0;
	while (data->trimmed[data->i])
	{
		if (data->trimmed[data->i] == '\'' || data->trimmed[data->i] == '\"')
		{
			quote = data->trimmed[data->i];
			data->i++;
			get_next_quote(quote, data);
		}
		else if (data->trimmed[data->i] == '|')
		{
			push_back(data, ft_strndup(&data->trimmed[count], data->i - count), 0);
			count = data->i + 1;
		}
		data->i++;
	}
	push_back(data, ft_strdup(&data->trimmed[count]), 0);
	preview(data);
	return (0);
}

//erjf " fre | fe" | er
/*
int	second_cut(t_data *data, t_token *token)
{
	t_token	*tmp;
	char	quote;
	int		count;

	tmp = token;
	data->i = 0;
	count = 0;
	while (tmp->value[data->i])
	{
		if (tmp->value[data->i] == '\'' || tmp->value[data->i] == '\"')
		{
			quote = tmp->value[data->i];
			data->i++;
			while (tmp->value[data->i] && tmp->value[data->i] != quote)
				data->i++;
		}
		else if (tmp->value[data->i] == ' ')
		{

		}
		data->i++;
	}
}*/

int	ft_count_words(t_data *data, char *s, char c)
{
//	int		i;
	int		count;
	char	quote;

//	i = 0;
	count = 0;
	data->i = 0;
	while (s[data->i] != '\0')
	{
		if (s[data->i] == c)
			data->i++;
		else if (s[data->i] == '\'' || s[data->i] == '\"')
		{
			count++;
			quote = s[data->i];
			data->i++;
			while (s[data->i] && s[data->i] != quote)
				data->i++;
		}
		else
		{
			count++;
			while (s[data->i] != '\0' && s[data->i] != c)
				data->i++;
		}
	}
	return (count);
}