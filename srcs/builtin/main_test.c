#include "../../minishell.h"

/*int	main(int argc, char **argv, char **envp)
{
	(void)argc;
//	(void)argv;
	t_params params;

//	char 	*input = "  echo 'lala lhho' lulu lili \"lili |lili\" | cat -e | pb";
//	char 	*input = "echo 'lala lhho' lulu lili \"lili |lili\" | cat| -e | pb  >> lulu|grep";
//    char *input = "pwd | env | ls | cat -e | cat -e ";
	t_call 	**calls;

	params.env = ft_get_env(envp);
	params.export = create_export(params.env);
//    ft_printf(input);
//    ft_printf("\n");
	if (parse(argv[1], &calls, &params))
        ft_printf("Error\n");
	free_export(params.export);
	free_table2(params.env);
//		ft_putstr_fd("Error\n", 2);
}*/

/*
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
//	(void)envp;
	char		**env;
	t_export	*export;

	env = ft_get_env(envp);
	export = create_export(env);

	env = ft_select_builtin(&argv[1], env, export);
//	env = ft_select_builtin(&argv[3], env, &export);
//	env = ft_select_builtin(&argv[4], env, &export);

	ft_printf("----------\nenv =\n");
	print_table(env);
	ft_printf("----------\nexport =\n");
	print_export(export);
	

	free_export(export);
	free_table(env);
	return (0);
}
*/

/*
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_export	*export;

	ft_printf("Demarrage de create_export\n");
	export = create_export(envp);
	ft_printf("Create_export termine\nDemarrage de print_export\n\n");
	print_export(export);
	ft_printf("\nPrint_export termine\n Demarrage de free_export\n");
	free_export(export);
	ft_printf("Free_export termine !\n");

	return (0);
}*/

int	main(int ac, char **av, char **envp)
{
	(void)av;
	(void)envp;
	t_data data;

	init_data(&data);
	if (ac == 1)
	{
		print_prompt(&data);
	}	
	return (EXIT_SUCCESS);
}
