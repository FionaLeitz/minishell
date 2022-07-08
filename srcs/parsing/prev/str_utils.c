#include "../../minishell.h"

int	is_in(char c, char *str)
{
	while (*str)
	{
		if (c == *str)
			return (1);
		str++;
	}
	return (0);
}

// Stops counting at the first element different that c
int	count_char_start(char c, char *str)
{
	int	count;
	
	count = 0;
	while (*str == c)
	{
		count++;
		str++;
	}
	return (count);
}

// Counts how many c there are in a string
int count_chars(char c, char *str)
{
	int	count;
	
	count = 0;
	while (*str)
	{
		if (*str == c)
			count++;
		str++;
	}
	return (count);
}

