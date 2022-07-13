/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_t_cmd_fill.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epresa-c <epresa-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 10:25:09 by epresa-c          #+#    #+#             */
/*   Updated: 2022/07/13 14:05:18 by epresa-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	fill_t_cmd(t_var *v, t_prompt *prompt, int k)
{
	static int	i = 0;
	t_cmd		*curr;
	int			redir_stat;
	int			j;
	int			op_rdir_stat;
	static int	n_pipe = 0;
	DIR*		dirptr;

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
	while (v->s_split[i] != NULL && v->s_split[i][0] != '|' && curr->exec_stat == EXECUTABLE)
	{
		redir_stat = FALSE;
		is_redir(v, &i, &j, &redir_stat, prompt);
		if (redir_stat != FALSE)
			fill_cmd_redir(v, &i, redir_stat, curr, &op_rdir_stat);
		else if (curr->exec_stat == EXECUTABLE)
		{
			curr->full_cmd = tab_add(curr->full_cmd, v->s_split[i]);
			is_builtin_is_exit(curr, prompt, i, v);
		}
		j++;
		i++;
	}
	if (curr->full_cmd && ft_strncmp(curr->full_cmd[0], "cd", 2) == 0 && \
		ft_strlen(curr->full_cmd[0]) == 2 && curr->full_cmd[1] == NULL)
		curr->full_cmd = cd_expantion_home(curr, prompt->envp);
	else if (curr->full_cmd && curr->full_cmd[0][0] == '/')
	{
		dirptr = opendir(curr->full_cmd[0]);
		if (access(curr->full_cmd[0], F_OK) == 0)
		{
			if (dirptr != NULL)
			{
				fn_echo_error(curr, v->s_split[i - j], "is a directory");
				closedir (dirptr);
				g_exit_status = 126;
			}
			else
				curr->full_path = ft_strdup(curr->full_cmd[0]);
		}
		else
		{
			fn_echo_error(curr, v->s_split[i - j], "No such file or directory");
			g_exit_status = 127;
		}
	}
	else if (curr->infile != -1 && curr->exec_stat == EXECUTABLE \
	&& curr->full_cmd != NULL)
	{
		if (curr->is_builtin == FALSE)
			curr->full_path = create_path(prompt->paths, curr->full_cmd[0]);
		if (curr->full_path == NULL && curr->is_builtin == FALSE)
			fn_echo_error(curr, v->s_split[i - j], "command not found");
	}
	if (v->s_split[i] == NULL)
		i = 0;
}

void	fill_cmd_redir(t_var *v, int *i, int redir_stat, t_cmd *curr, int *op_rdir_stat)
{
	struct termios	termios_save;
	struct termios	termios_new;

	*op_rdir_stat = FALSE;
	if (redir_stat == REDIR_OUTPUT_APPEND)
		*op_rdir_stat = open_outfiles(v->s_split[*i], TRUE, curr);
	else if (redir_stat == REDIR_OUTPUT_SIMPLE)
		*op_rdir_stat = open_outfiles(v->s_split[*i], FALSE, curr);
	else if (redir_stat == HERE_DOC)
	{
		tcgetattr(0, &termios_save);
		termios_new = termios_save;
		termios_new.c_lflag &= ~ECHOCTL;
		tcsetattr(0, 0, &termios_new);
		*op_rdir_stat = open_in_files(NULL, v->s_split[*i], curr);
		tcsetattr(0, 0, &termios_save);
	}
	else if (redir_stat == REDIR_INPUT)
		*op_rdir_stat = open_in_files(v->s_split[*i], NULL, curr);
	if (*op_rdir_stat == FALSE)
		fn_echo_error(curr, v->s_split[*i], "no such a file or directory");
}
