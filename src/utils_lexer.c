/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epresa-c <epresa-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:34:47 by epresa-c          #+#    #+#             */
/*   Updated: 2022/06/17 15:47:13 by epresa-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_prompt(char **envp)
{
	char	*str_get_info;
	char	*str_prompt;

	str_get_info = get_env("USER", envp);
	str_prompt = ft_strjoin(str_get_info, "@minishell42$ ");
	return (str_prompt);
}

void	init_t_var_main(t_var *v)
{
	v->line = NULL;
	v->split = NULL;
	v->tmp = NULL;
	v->subsplit = NULL;
	v->i = 0;
	v->j = 0;
}

void	init_t_prompt(t_prompt *prompt, char **envp)
{
	prompt->cmds = NULL;
	prompt->envp = init_envp(envp);
	prompt->pid = NULL;
	prompt->prompt_text = get_prompt(prompt->envp);
	prompt->n_cmds = 0;
}

void	print_tab_with_str_name(char **tab, char *tab_name)
{
	int	i;

	i = 0;
	if (tab == NULL)
		return ;
	ft_printf("\t Printing %s\n", tab_name);
	while (tab && tab[i])
	{
		ft_printf("%s[%d] = %s\n", tab_name, i, tab[i]);
		i++;
	}
}
