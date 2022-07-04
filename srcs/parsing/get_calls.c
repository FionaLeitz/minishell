#include "../../minishell.h"

// Count the number of pipe elements in a list of elements -> +1 = nb of calls
int	count_pipes(char **elements)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!elements)
		return (count);
	while (elements[i])
	{
		if (ft_strlen(elements[i]) == 1 && elements[i][0] == '|')
			count++;
		i++;
	}
	return (count);
}

// Get nb of elements until next syntax element
int len_portion(char **elements)
{
    int j;

    j = 0;
    while (elements[j])
    {
        if (is_syntax(elements[j]))
            return (j);
        j++;
    }
	return (j);
}

int	get_next_call(int *start, char **elements, t_call **call)
{
    int j;

    j = 0;
	(*call)->program = ft_strdup(elements[*start]);
    if (!(*call)->program)
	{
		return (1);
	}
//	ft_printf("start : %d\n", *start);
//	ft_printf("element : %s\n", elements[*start]);
    j = len_portion(elements + *start);
//	ft_printf("j : %d\n", j);
    if (table_dup(elements + *start, j, &((*call)->args)))
	{
		return (1);
	}
	*start += j;
    if (get_funnels(start, elements, call))
		return (1);
    return (0);
}

int	get_calls(int n_elements, char **elements, t_call ***calls)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	j = 0;
    (void )n_elements;
	count = count_pipes(elements) + 1;
	if (check_calls(elements))	
		return (1);
	*calls = (t_call **)malloc(sizeof(t_call *) * (count + 1));
	if (!(*calls))
		return (1);
	while (j < count)
	{
		(*calls)[j] = (t_call *)malloc(sizeof(t_call));
		if (!((*calls)[j]))
			return (1);
		get_next_call(&i, elements, *calls + j);
		i++;
		j++;
	}
	(*calls)[j] = NULL;
	return (0);
}
