#include "../../minishell.h"

int	free_parsing(t_call **calls, char **elements, int size_elements)
{
	int	n_calls;

	if (elements)
	{
		n_calls = count_pipes(elements) + 1;
		free_table(elements, size_elements);
	}
	if (calls)
		free_calls(calls, n_calls);
	return (1);
}

int	parse(char *input, t_call ***calls, t_params *params)
{
    char **elements;
	int	n_elements;

	n_elements = 0;
	if (check_quotes(input))
		return (1);
	if (get_elements(input, &n_elements, &elements))
		return (free_parsing(NULL, elements, n_elements));
	if (get_calls(n_elements, elements, calls))
		return (free_parsing(*calls, elements, n_elements));
//	print_calls(*calls);

	command(*calls, params);

	free_parsing(*calls, elements, n_elements);
    return (0);
}
/*
int	main(void)
{
	char 	*input = "  echo 'lala lhho' lulu lili \"lili |lili\" | cat -e | pb";
	char 	*input = "echo 'lala lhho' lulu lili \"lili |lili\" | cat| -e | pb  >> lulu|grep";
    char *input = "echo lala lulu > hello | grep thing";
	t_call 	**calls;
	t_params	*params;

	if (parse(input, &calls, &params))
        ft_printf("Error\n");
}*/
