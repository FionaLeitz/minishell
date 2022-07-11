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