/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Emiliano <Emiliano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:01:33 by Emiliano          #+#    #+#             */
/*   Updated: 2022/07/12 11:06:09 by Emiliano         ###   ########.fr       */
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
		free(curr->prev->status);
		curr->prev->status = NULL;
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
	// g_exit_status = 0;
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
	tab_free(v->s_split);
	free(v->s_split);
	v->s_split = NULL;
	free_t_cmd(&prompt->cmds);
	tab_free(prompt->paths);
	free(prompt->paths);
	prompt->paths = NULL;
	prompt->n_cmds = 1;
}

int	check_line(char *line)
{
	int	i;

	i = 0;
	if (ft_strlen(line) == 0)
		return (FALSE);
	while (line[i])
	{
		if (32 < (int)line[i] && (int)line[i] < 127)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

int	main(int argc, char **argv, char **envp)
{
	t_var			v;
	t_prompt		prompt;
	struct termios	termios_save;
	struct termios	termios_new;

	(void)argv;
	(void)argc;
	init_mini_vars(&v, &prompt, envp);
	while (prompt.stop == FALSE)
	{
		g_exit_status = 0;
		signal(SIGQUIT, SIG_IGN); // ctrl + '\'
		signal(SIGINT, signal_handler1); // ctrl + C
		tcgetattr(0, &termios_save);
		termios_new = termios_save;
		termios_new.c_lflag &= ~ECHOCTL;
		tcsetattr(0, 0, &termios_new);
		v.line = readline(prompt.prompt_text);
		tcsetattr(0, 0, &termios_save);
		if (!v.line)
			prompt.stop = TRUE;
		else if (check_line(v.line) == TRUE)
		{
			signal(SIGINT, signal_handler2); // ctrl + C
			add_history(v.line);
			fn_lexer(&v, &prompt);
			if (v.split != NULL && prompt.token_status != FAILED)
			{
				fn_parsing(&v, &prompt);
				if (prompt.token_status != FAILED)
				{
					// print_list(&prompt); // << TO DELETE: it's just to print the list
					read_list(&prompt);
				}
			}
		}
		free_all_tabs_and_prompt(&v, &prompt);
	}
	write(1, "exit\n", 6);
	exit (g_exit_status);
}
