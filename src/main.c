/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Emiliano <Emiliano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:01:33 by Emiliano          #+#    #+#             */
/*   Updated: 2022/06/18 11:36:09 by Emiliano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init(t_var *v, t_prompt *prompt, char **envp)
{
	g_exit_status = 0;
	init_t_var_main(v);
	init_t_prompt(prompt, envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_var		v;
	t_prompt	prompt;

	(void)argv;
	init(&v, &prompt, envp);
	// init_t_var_main(&v);
	// init_t_prompt(&prompt, envp);
	signal(SIGINT, signal_handler); // ctrl + C
	signal(SIGQUIT, signal_handler); // ctrl + '\'
	// CTRL+D = EOF of standard input, wich ends the proces; LEARN HOW TO HANDLE THIS
	while (argc == 1)
	{
		v.line = readline(prompt.prompt_text);
        add_history(v.line);
		fn_lexer(&v, &prompt);
		//fn_parsing(&v, &prompt);
		free_all_tabs(&v);
	}
	return (0);
}
