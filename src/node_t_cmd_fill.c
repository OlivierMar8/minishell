/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_t_cmd_fill.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epresa-c <epresa-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 10:25:09 by epresa-c          #+#    #+#             */
/*   Updated: 2022/07/07 14:35:34 by epresa-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	fill_t_cmd(t_var *v, t_prompt *prompt, int k)
{
	static int	i = 0;
	t_cmd		*curr;
	int			redir_status;
	int			j;
	int			open_redir_status;
	static int	n_pipe = 0;

	j = 0;
	if (k == 0)
		n_pipe = 0;
	if (k == 0)
		i = 0;
	curr = prompt->cmds;
	while (curr->next != NULL)
			curr = curr->next;
	if (v->s_split[i][0] == '|')
	{
		if (curr->prev->outfile == 1)
		{
			curr->prev->outfile = prompt->pipes[n_pipe][1];
			curr->infile = prompt->pipes[n_pipe][0];
			n_pipe++;
		}
		i++;
	}
	while (v->s_split[i] != NULL && v->s_split[i][0] != '|')
	{
		redir_status = FALSE;
		is_redir(v, &i, &j, &redir_status, prompt);
		if (redir_status != FALSE)
			fill_cmd_with_redir(v, &i, redir_status, curr, &open_redir_status);
		else
		{
			curr->full_cmd = tab_add(curr->full_cmd, v->s_split[i]);
			is_builtin_is_exit(curr, prompt, i, v);
		}
		j++;
		i++;
	}
	if (ft_strncmp(curr->full_cmd[0], "cd", 2) == 0 && \
		ft_strlen(curr->full_cmd[0]) == 2 && curr->full_cmd[1] == NULL)
		curr->full_cmd = cd_expantion_home(curr, prompt->envp);
	else if (curr->full_cmd[0][0] == '/')
	{
		if (access(curr->full_cmd[0], F_OK) == 0)
			fn_echo_error(curr, v->s_split[i - j], "is a directory");
		else
			fn_echo_error(curr, v->s_split[i - j], "No such file or directory");
	}
	if (curr->infile != -1 && curr->exec_stat == EXECUTABLE)
	{
		if (curr->full_cmd != NULL && curr->is_builtin == FALSE)
			curr->full_path = create_path(prompt->paths, curr->full_cmd[0]);
		if (curr->full_path == NULL && curr->is_builtin == FALSE)
			fn_echo_error(curr, v->s_split[i - j], "command not found");
	}
	if (v->s_split[i] == NULL)
		i = 0;
}

void	fill_cmd_with_redir(t_var *v, int *i, int redir_status, t_cmd *curr, int *open_redir_status)
{
	*open_redir_status = FALSE;
	if (redir_status == REDIR_OUTPUT_APPEND)
	{
		*open_redir_status = open_outfiles(v->s_split[*i], TRUE, curr);
		if (*open_redir_status == FALSE)
			fn_echo_error(curr, v->s_split[*i], "No such a file or directory");
	}
	else if (redir_status == REDIR_OUTPUT_SIMPLE)
		*open_redir_status = open_outfiles(v->s_split[*i], FALSE, curr);
	else if (redir_status == HERE_DOC)
		*open_redir_status = open_in_files(NULL, v->s_split[*i], curr);
	else if (redir_status == REDIR_INPUT)
		*open_redir_status = open_in_files(v->s_split[*i], NULL, curr);
	if (*open_redir_status == FALSE)
		fn_echo_error(curr, v->s_split[*i], "no such a file or directory");
}
