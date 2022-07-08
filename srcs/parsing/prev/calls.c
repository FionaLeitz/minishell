#include "../../minishell.h"

void    free_funnel_table(t_funnel **funnels)
{
    int i;

    i = 0;
    while (funnels[i])
    {
        free_funnel(funnels[i]);
        free(funnels[i]);
        i++;
    }
    free(funnels);
}

void	free_call(t_call call)
{
    int size;

    size = get_size_table(call.args);
	free(call.program);
	free_table(call.args, size);
    free_funnel_table(call.funnels);
}

// Free list of calls provided list and size list
void	free_calls(t_call **calls, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
        free_call(*(calls[i]));
		free(calls[i]);
		i++;
	}
	free(calls);
}

void	print_call(t_call call)
{
	int	i;

	i = 0;
    ft_printf("===========================\n");
	ft_printf("Program : %s\n", call.program);
	ft_printf("Arguments :\n");
	print_table(call.args);
	ft_printf("Funnels : \n");
	while ((call.funnels)[i])
	{
        ft_printf("--- funnel %d ---\n", i);
		print_funnel(*((call.funnels)[i]));
		i++;
	}
}

void	print_calls(t_call **calls)
{
	int	i;

	i = 0;
	while (calls[i])
	{
		print_call(*(calls[i]));
        i++;
	}
}
