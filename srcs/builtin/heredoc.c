#include "../../minishell.h"

static t_end	*new_line(char *str)
{
	t_end	*new;

	new = malloc(sizeof(t_end));
	if (new == NULL)
		return (NULL);
	new->str = strdup(str);
	free(str);
	new->next = NULL;
	return (new);
}

static int	end_heredoc(t_end *end)
{
	int	i;
	t_end	*save;

	if (end == NULL)
		return (1);
	while (end)
	{
		free(end->str);
		save = end;
		end = end->next;
		free(save);
	}
	return (1);
}

int	heredoc(char **arg)
{
	char	*str;
	t_end	*end;
	t_end	*save;

	str = readline("heredoc>");
	if (strcmp(str, arg[1]) == 0)
	{
		free(str);
		return (1);
	}
	end = new_line(str);
	save = end;
	while (1)
	{
		str = readline("heredoc>");
		if (strcmp(str, arg[1]) == 0/* || str == NULL*/)
			break ;
		end->next = new_line(str);
		end = end->next;
		free(str);
	}
	free(str);
	end = save;
	while (end)
	{
		printf("%s\n", end->str);
		end = end->next;
	}
	end_heredoc(save);
	return (0);
}