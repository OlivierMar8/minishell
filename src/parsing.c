/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Emiliano <Emiliano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 10:25:09 by epresa-c          #+#    #+#             */
/*   Updated: 2022/06/23 10:52:59 by Emiliano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*start_t_cmd(t_prompt *prompt)
{
	t_cmd	*new_node;

	new_node = malloc(sizeof(t_cmd));
	if (new_node == NULL)
		ft_printf("Error malloc: new node creation has failed\n");
	else
	{
		new_node->full_cmd = NULL;
		new_node->full_path = NULL;
		new_node->infile = STDIN_FILENO;
		new_node->outfile = STDOUT_FILENO;
		new_node->is_builtin = FALSE;
		new_node->status = 0;
		new_node->next = NULL;
		new_node->prev = NULL;
		prompt->cmds = new_node;
	}
	return (new_node);
}

void    add_t_cmd(t_prompt *prompt)
{
	t_cmd	*new_node;
	t_cmd	*curr;

	curr = prompt->cmds;
	new_node = malloc(sizeof(t_cmd));
	while (curr->next != NULL)
		curr = curr->next;
	if (new_node == NULL)
		ft_printf("Error malloc: new node creation has failed\n");
	else
	{
		new_node->full_cmd = NULL;
		new_node->full_path = NULL;
		new_node->infile = STDIN_FILENO;
		new_node->outfile = STDOUT_FILENO;
		new_node->is_builtin = FALSE;
		new_node->status = 0;
		new_node->next = NULL;
		new_node->prev = curr;
		curr->next = new_node;
	}
}

int	count_cmds(t_prompt *prompt)
{
	t_cmd	*curr;
	int i;

	i = 0;
	curr = prompt->cmds;
	while (curr != NULL)
	{
		curr = curr->next;
		i++;
	}
	return (i);
}

void	fill_t_cmd(t_var *v, t_prompt *prompt)
{
	int	i;
    // int next_pipe_index;
    t_cmd	*curr;   
    
    // next_pipe_index = 0;
    curr = prompt->cmds;
    while (curr->next != NULL)
            curr = curr->next;
    i = 0;
	while (v->subsplit[i] != NULL && v->subsplit[i][0] != '|')
	{
		curr->full_cmd = tab_add(curr->full_cmd, v->subsplit[i]);
		i++;
	}
	curr->full_path = create_path(prompt->paths, curr->full_cmd[0]);
}

void	fn_parsing(t_var *v, t_prompt *prompt)
{
    int i;

    if (v->subsplit == NULL)
        return ; 
// TO FIX: when subsplit[0][0] == '|' segfault
    i = 0;
    while (v->subsplit[i] && v->subsplit[i + 1] != NULL)
    {
        if (v->subsplit[i][0] == '|')
            prompt->n_cmds++;
        i++;
    }
	ft_printf("\nn_cmd = %d\n", prompt->n_cmds);
    i = 0;
    while (i < prompt->n_cmds)
    {
        if (i == 0)
            prompt->cmds = start_t_cmd(prompt);
        if (i > 0)
            add_t_cmd(prompt);
        fill_t_cmd(v, prompt);      
        i++;
    }
}

void	print_list(t_prompt *prompt)
{
	t_cmd	*curr;
	int		i;

	i = 0;
	curr = prompt->cmds;
	while (curr != NULL)
	{
		ft_printf("\n\t**** PRINTING LIST OF COMMANDS ****\n\n");
		ft_printf("cmd_[%d] \t address = %p\n", i, curr);
		print_tab_with_str_name(curr->full_cmd, "cmd->full_cmd");
		ft_printf("\tfull_path = %s\n", curr->full_path);
		ft_printf("\tinfile = %d\n", curr->infile);
		ft_printf("\toutfile = %d\n", curr->outfile);
		ft_printf("\tis_builtin = %d\n", curr->is_builtin);
		ft_printf("\tstatus = %d\n", curr->status);
		ft_printf("\tprev = %p\n", curr->prev);
		ft_printf("\tnext = %p\n", curr->next);
		curr = curr->next;
		i++;
	}
}