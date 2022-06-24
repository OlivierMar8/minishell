/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epresa-c <epresa-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:01:33 by Emiliano          #+#    #+#             */
/*   Updated: 2022/06/24 15:45:55 by olmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_t_cmd(t_cmd **cmd)
{
	t_cmd	*curr;

	curr = *cmd;
	if (*cmd == NULL)
		return ;
	while (curr->next != NULL)
	{
		curr = curr->next;
        tab_free(curr->prev->full_cmd);
        free(curr->prev->full_cmd);
        curr->prev->full_cmd = NULL;
        free(curr->prev->full_path);
        curr->prev->full_path = NULL;
		free(curr->prev);
        curr->prev = NULL;
	}
    tab_free(curr->full_cmd);
    free(curr->full_cmd);
    curr->full_cmd = NULL;
    free(curr->full_path);
    curr->full_path = NULL;
	free(curr);
	*cmd = NULL;
}

void	init_mini_vars(t_var *v, t_prompt *prompt, char **envp)
{
	g_exit_status = 0;
	init_t_var_main(v);
	init_t_prompt(prompt, envp);
}

void	free_all_tabs_and_prompt(t_var *v, t_prompt *prompt)
{
	free(v->line);
	v->line = NULL;
	tab_free(v->split);
    free(v->split);
	v->split = NULL;
	tab_free(v->subsplit);
    free(v->subsplit);
	v->subsplit = NULL;
    free_t_cmd(&prompt->cmds);

    // problems with the prompter after execute next two lines
    // free(prompt->prompt_text);
    // prompt->prompt_text = NULL;
    tab_free(prompt->paths);
    free(prompt->paths);
    prompt->paths = NULL;
    prompt->n_cmds = 1;
}

int	main(int argc, char **argv, char **envp)
{
	t_var		v;
	t_prompt	prompt;

	(void)argv;
	init_mini_vars(&v, &prompt, envp);
	signal(SIGINT, signal_handler); // ctrl + C
	signal(SIGQUIT, SIG_IGN); // ctrl + '\'
	// CTRL+D = EOF of standard input, wich ends the proces; LEARN HOW TO HANDLE THIS
	while (argc == 1)
	{
		v.line = readline(prompt.prompt_text);
		if (ft_strncmp(v.line, "exit", 4) == 0 && ft_strlen(v.line) == 4)
			ft_exit(&v, &prompt);
		if (ft_strlen(v.line) != 0)
		{
			add_history(v.line);
			fn_lexer(&v, &prompt);
			if (v.split != NULL)
			{
				fn_parsing(&v, &prompt);
				//print_list(&prompt); // just to print the list
				read_list(&prompt);
			}
		}
		free_all_tabs_and_prompt(&v, &prompt);
	}
	return (0);
}
