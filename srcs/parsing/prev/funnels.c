#include "../../minishell.h"

void    free_funnel(t_funnel *funnel)
{
    free(funnel->type_funnel);
    free_table(funnel->file_names, get_size_table(funnel->file_names));
}

void	print_funnel(t_funnel funnel)
{
	ft_printf("Type : %s\n", funnel.type_funnel);
	ft_printf("Files : \n");
	print_table(funnel.file_names);
}
