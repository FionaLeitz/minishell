#include "../../minishell.h"

// Utility for next func
int quote_nb(char c)
{
	if (c == '\'')
		return (0);
	else if (c == '\"')
		return (1);
	return (0);
}

int get_size_next_element(char *input)
{
    int size;
	int	btw_quotes[2]; // First el ' second "

	btw_quotes[0] = 0;
	btw_quotes[1] = 0;
    size = 0;
	input += count_char_start(' ', input);
	if (is_in(*input, "|<>"))
		return (count_char_start(*input, input));
	while (*input)
	{
		if (*input == '\'' || *input == '\"')
            btw_quotes[quote_nb(*input)] = 1 - btw_quotes[quote_nb(*input)];
		else if (is_in(*input, " |<>") && !btw_quotes[0] && !btw_quotes[1])
			return (size);
		size++;
		input++;
	}
	return (size);
}

int count_args(char *input)
{
    int count;
	int	btw_quotes[2]; // First el ' second "

	btw_quotes[0] = 0;
	btw_quotes[1] = 0;
    count = 0;
    while (*input == ' ')
        input++;
    while (*input)
    {
        while (*input && (!(is_in(*input, " ><|")) || btw_quotes[0] || btw_quotes[1]))        
        {
            if (*input == '\'' || *input == '\"')
                btw_quotes[quote_nb(*input)] = 1 - btw_quotes[quote_nb(*input)];
            input++;
        }
        while (is_in(*input, " ><|"))        
        {
            input++;
        }
        count++;
    }
    return (count);
}

int count_syntax(char *input)
{
    int     count;
    char    c;
	int	btw_quotes[2]; // First el ' second "

	btw_quotes[0] = 0;
	btw_quotes[1] = 0;
    count = 0;
    c = 0;
    while (*input)
    {
		if (*input == '\'' || *input == '\"')
            btw_quotes[quote_nb(*input)] = 1 - btw_quotes[quote_nb(*input)];
		if (is_in(*input, "|<>") && !btw_quotes[0] && !btw_quotes[1])
        {
            c = *input;
            while (*input == c)
                input++;
            count++;
        }
        input++;
    }
    return (count);
}

int count_elements(char *input)
{
    int total;

    total = count_args(input) + count_syntax(input);
    return (total);
}

// parse elements in table 
int	get_elements(char *input, int *n_elements, char ***table_elements)
{
    int size;
    int i;
    
	*n_elements = count_elements(input);
	*table_elements = (char **)malloc(sizeof(char *) * (*n_elements + 1));  
	if (!(*table_elements))
		return (1);
    size = get_size_next_element(input);
    i = 0;
    while (i < *n_elements)
    {
		input += count_char_start(' ', input);
        (*table_elements)[i] = ft_strndup(input, size);            
        if (!((*table_elements)[i]))
        {
            free_table((*table_elements), i);
            return (1);
        }
		input += size;
        size = get_size_next_element(input);
        i++;
    }
	(*table_elements)[i] = NULL;
	return (0);
}
