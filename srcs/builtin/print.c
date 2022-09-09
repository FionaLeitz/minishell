
#include "../../minishell.h"

// if env, print the environment unless arguments
int	ft_env(char **arg, t_params *params)
{
	if (arg[1] != NULL)
	{
		ft_putstr_fd("env: ‘", 2);
		ft_putstr_fd(arg[1], 2);
		ft_putstr_fd("’: too many arguments\n", 2);
		return (127);
	}
	if (params->env == NULL)
		ft_printf("\n");
	else
		print_table(params->env);
	return (0);
}

// print export if no argument
int	print_export(t_params *params)
{
	t_export	*tmp;
	int			i;

	tmp = params->export;
	while (params->export)
	{
		if (params->export->name[0] != '\0')
		{
			ft_printf("declare -x %s=\"", params->export->name);
			i = 0;
			while (params->export->value[i] && params->export->value[++i])
			{
				if (params->export->value[i] == -1)
					ft_printf("\'");
				else if (params->export->value[i] == -2)
					ft_printf("\"");
				else
					ft_printf("%c", params->export->value[i]);
			}
			ft_printf("\"\n");
		}
		params->export = params->export->next;
	}
	params->export = tmp;
	return (0);
}
