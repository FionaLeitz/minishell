#include "../../minishell.h"

// Check if the string contains | < > not between quotes
int	has_unquoted_syntax(char *str)
{
	char	opened_quote;

	opened_quote = 0;
	while (*str)
	{
		if (is_in(*str, "\"\'") && opened_quote == 0)
			opened_quote = *str;
		else if (opened_quote == *str)
			opened_quote = 0;
		else
		{
			if (opened_quote == 0 && is_in(*str, "|<>"))
				return (1);
		}
		str++;
	}
	return (0);
}

// Check if element is syntax (| or << or >> or < or >)
int	is_syntax(char *str)
{
	if (ft_strlen(str) == 1 && is_in(*str, "|><"))
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

// Check if element is invalid (meaning contains syntax without being a valid syntax element)
int	is_invalid(char *str)
{
	if (has_unquoted_syntax(str) && !is_syntax(str))
		return (1);
	if (ft_strlen(str) == 0)
		return (1);
	return (0);
}

// Check if calls properly alternate syntax and programs
int	check_calls(char **table)
{
	int	i;
	int	last_syntax;

	i = 0;
	last_syntax = 1;
	while (table[i])
	{
		if (is_invalid(table[i]))
			return (1);
		if (is_syntax(table[i]))
		{
			if (last_syntax)
				return (1);
			last_syntax = 1;
		}
		else if (!is_syntax(table[i]))
			last_syntax = 0;
		i++;
	}
	return (0);
}
