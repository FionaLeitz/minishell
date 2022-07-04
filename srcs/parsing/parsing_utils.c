#include "../../minishell.h"

// Checks if quotes go 2 by 2
int check_quotes(char *str)
{
	char	opened;

	opened = 0;
	while (*str)
	{
		if (is_in(*str, "\"\'") && opened != 0)
			opened = *str;
		else if (*str == opened)
			opened = 0;
		str++;
	}
	if (opened != 0)
		return (1);
	return (0);
}
