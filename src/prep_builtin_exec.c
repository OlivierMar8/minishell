/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_builtin_exec.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olmartin <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 15:57:08 by olmartin          #+#    #+#             */
/*   Updated: 2022/07/01 15:03:08 by olmartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	search_builtin(t_prompt *s_pr, t_cmd *cur_cmd, int num)
{
	int	res;

	(void)s_pr;
	(void)num;
	res = 0;
	if (ft_strncmp(cur_cmd->full_cmd[0], "cd", 3) == 0)
		res = cd_builtin(cur_cmd, s_pr);
	else if (ft_strncmp(cur_cmd->full_cmd[0], "echo", 5) == 0)
		echo_builtin(cur_cmd);
	else if (ft_strncmp(cur_cmd->full_cmd[0], "env", 4) == 0)
		env_builtin(s_pr);
	else if (ft_strncmp(cur_cmd->full_cmd[0], "export", 7) == 0)
		s_pr->envp = export_builtin(cur_cmd, s_pr);
	else if (ft_strncmp(cur_cmd->full_cmd[0], "pwd", 4) == 0)
		res = pwd_builtin(s_pr->envp);
	else if (ft_strncmp(cur_cmd->full_cmd[0], "unset", 6) == 0)
		s_pr->envp = unset_builtin(cur_cmd->full_cmd[1], s_pr);
	return (res);
}

void	redir_builtin(t_prompt *s_pr, t_cmd *cur_cmd, int num)
{
	int	dup_res;

	dup_res = -1;
/*	int		dup_res[2];

	dup_res[0] = -1;
	dup_res[1] = -1;
	printf("in fd et out fd: %d - %d \n", cur_cmd->infile, cur_cmd->outfile);
	if (cur_cmd->infile != 0)
	{
		dup_res[1] = dup2(cur_cmd->infile, STDIN_FILENO);
		if (dup_res[1] < 0)
			perror("Error with dup gen.");
	}
*/	if (cur_cmd->outfile != 0)
	{
		//dup_res[0] = dup2(cur_cmd->outfile, STDOUT_FILENO);
		//if (dup_res[0] < 0)
		dup_res = dup2(cur_cmd->outfile, STDOUT_FILENO);
		if (dup_res < 0)
			perror("Error with dup gen.");
	}
	close_pipes(s_pr);
	search_builtin(s_pr, cur_cmd, num);
}

int	fork_builtin(t_prompt *s_pr, t_cmd *cur_cmd, int num)
{
	int	exitstatus;

	s_pr->pid[num] = fork();
	if (s_pr->pid[num] < 0)
	{
		perror("Error fork");
		return (0);
	}
	if (s_pr->pid[num] == 0)
	{
		redir_builtin(s_pr, cur_cmd, num);
		exit (g_exit_status);
	}
	else
	{
		waitpid(s_pr->pid[num], &exitstatus, 0);
		wait_status(exitstatus);
	}
	if (cur_cmd->infile != 0)
		close(cur_cmd->infile);
	if (cur_cmd->outfile != 1)
		close(cur_cmd->outfile);
	//if (num == s_pr->n_cmds)
	//	close_pipes(s_pr);
	return (0);
}

void	builtin_close_redir(t_cmd *cur_cmd)
{
	if (cur_cmd->infile != 0)
		close(cur_cmd->infile);
	if (cur_cmd->outfile != 1)
		close(cur_cmd->outfile);
}


int	builtin_is_redir(t_prompt *s_pr, t_cmd *cur_cmd, int num)
{
	char	*cmd;

	cmd = cur_cmd->full_cmd[0];
	if (ft_strncmp(cmd, "cd", 3) == 0 || ft_strncmp(cmd, "export", 7) == 0 || \
		ft_strncmp(cmd, "unset", 6) == 0)
	{
		builtin_close_redir(cur_cmd);
		return (search_builtin(s_pr, cur_cmd, num));
	}
	else if (cur_cmd->outfile != 1)
		return (fork_builtin(s_pr, cur_cmd, num));
	else
		return (search_builtin(s_pr, cur_cmd, num));
}
