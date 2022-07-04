#include "../../minishell.h"

// Print string table
void	print_table(char **table)
{
	int	i;

	i = 0;
    if (table)
    {
        while (table[i])
        {
            ft_printf(table[i]);
            ft_printf("\n");
            i++;
        }
    }
}

// Free table based on size
void    free_table(char **table, int size)
{
    int i;

    i = 0;
    while (i < size)
    {
        free(table[i]);
        i++;
    }
    free(table);
}

// Get size table
int get_size_table(char **table)
{
    int i;

    i = 0;
    while (table[i])
        i++;
    return (i);
}

// Duplicates a table of strings
int table_dup(char **table, int size, char ***new_table)
{
    int i;

    *new_table = (char **)malloc(sizeof(char *) * (size + 1));
    i = 0;
    while (i < size)
    {
       (*new_table)[i] = ft_strdup(table[i]); 
       if (!(*new_table)[i])
       {
           free_table(table, i);
           return (1);
       }
       i++;
    }
	(*new_table)[i] = NULL;
    return (0);
}
