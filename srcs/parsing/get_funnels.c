#include "../../minishell.h"

int	is_funnel(char *str)
{
	if (ft_strlen(str) == 1 && is_in(*str, "><"))
		return (1);
	if (ft_strlen(str) == 2)
	{
		if (str[0] == '>' && str[1] == '>')
			return (1);
		if (str[0] == '<' && str[1] == '<')
			return (1);
	}
	return (0);
}

int	count_funnels(char **elements)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!elements[i])
		return (count);
	while (!(is_syntax(elements[i])) || is_funnel(elements[i]))
	{
		if (is_funnel(elements[i]))
			count++;
		i++;
	}
	return (count);
}

int	get_next_funnel(int *start, char **elements, t_call **call, int i)
{
	t_funnel	*funnel;
	int			len;

	((*call)->funnels)[i] = (t_funnel *)malloc(sizeof(t_funnel));
	funnel = ((*call)->funnels)[i];
	if (!(funnel))
		return (1);
	funnel->type_funnel = ft_strdup(elements[*start]);
	len = len_portion(elements + *start + 1);
    if (table_dup(elements + *start + 1, len, &(funnel->file_names)))
	{
		return (1);
	}
	*start += len + 1;
	return (0);
}

int	get_funnels(int *start, char **elements, t_call **call)
{
	int	count;
	int	i;

	i = 0;
	count = count_funnels(elements + *start);
	(*call)->funnels = (t_funnel **)malloc(sizeof(t_funnel) * (count + 1));
	if (!((*call)->funnels))
		return (1);
	while (i < count)
	{
		if (get_next_funnel(start, elements, call, i))
			return (1);
		i++;
	}
	((*call)->funnels)[i] = NULL;
	return (0);
}
