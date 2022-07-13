/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtin_and_redir.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epresa-c <epresa-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 10:25:09 by epresa-c          #+#    #+#             */
/*   Updated: 2022/07/13 14:04:44 by epresa-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_builtin(char *cmd)
{
	int	is_builtin;

	is_builtin = FALSE;
	if (ft_strncmp(cmd, "echo", 4) == 0 && ft_strlen(cmd) == 4)
		is_builtin = TRUE;
	if (ft_strncmp(cmd, "cd", 2) == 0 && ft_strlen(cmd) == 2)
		is_builtin = TRUE;
	if (ft_strncmp(cmd, "pwd", 3) == 0 && ft_strlen(cmd) == 3)
		is_builtin = TRUE;
	if (ft_strncmp(cmd, "export", 6) == 0 && ft_strlen(cmd) == 6)
		is_builtin = TRUE;
	if (ft_strncmp(cmd, "unset", 5) == 0 && ft_strlen(cmd) == 5)
		is_builtin = TRUE;
	if (ft_strncmp(cmd, "env", 3) == 0 && ft_strlen(cmd) == 3)
		is_builtin = TRUE;
	if (ft_strncmp(cmd, "exit", 4) == 0 && ft_strlen(cmd) == 4)
		is_builtin = TRUE;
	return (is_builtin);
}

void	is_builtin_is_exit(t_cmd *curr, t_prompt *prompt, int i, t_var *v)
{
	if (check_builtin(curr->full_cmd[0]) == TRUE)
		curr->is_builtin = TRUE;
	if (ft_strncmp(curr->full_cmd[0], "exit", 4) == 0 && \
		ft_strlen(curr->full_cmd[0]) == 4)
	{
		if (i == 0)
		{
			prompt->stop = TRUE;
			if (v->s_split[i + 1])
				g_exit_status = ft_atoi(v->s_split[i + 1]) % 256;
		}
		curr->exec_stat = NON_EXECUTABLE;
	}
	else
		prompt->stop = FALSE;
}

void	is_redir(t_var *v, int *i, int *j, int *redir_stat, t_prompt *prompt)
{
	if (v->s_split[*i][0] == '>' && v->s_split[*i + 1] \
	&& v->s_split[*i + 1][0] == '>')
	{
		if (v->s_split[*i + 2] != NULL && \
		ft_strchr("~%^{}:; |\\", v->s_split[*i + 2][0]) == 0)
		{
			*redir_stat = REDIR_OUTPUT_APPEND;
			*i += 2;
			*j += 2;
		}
		else
		{
			prompt->token_status = FAILED;
			prompt->error_msg = SYNTAX_ERROR_NEAR_UNEXPECTED_TOKEN;
			print_error(prompt);
		}
		return ;
	}
	if (v->s_split[*i][0] == '<' && v->s_split[*i + 1] \
	&& v->s_split[*i + 1][0] == '<')
	{
		if (v->s_split[*i + 2] != NULL && \
		ft_strchr("~%^{}:; |\\", v->s_split[*i + 2][0]) == 0)
		{
			*redir_stat = HERE_DOC;
			*i += 2;
			*j += 2;
		}
		else
		{
			prompt->token_status = FAILED;
			prompt->error_msg = SYNTAX_ERROR_NEAR_UNEXPECTED_TOKEN;
			print_error(prompt);
		}
		return ;
	}
	if (v->s_split[*i][0] == '>')
	{
		if (v->s_split[*i + 1] != NULL && \
		ft_strchr("~%^{}:; |\\", v->s_split[*i + 1][0]) == 0)
		{
			*redir_stat = REDIR_OUTPUT_SIMPLE;
			*i += 1;
			*j += 1;
		}
		else
		{
			prompt->token_status = FAILED;
			prompt->error_msg = SYNTAX_ERROR_NEAR_UNEXPECTED_TOKEN;
			print_error(prompt);
		}
	}
	if (v->s_split[*i][0] == '<')
	{
		if (v->s_split[*i + 1] != NULL && \
		ft_strchr("~%^{}:; |\\", v->s_split[*i + 1][0]) == 0)
		{
			*redir_stat = REDIR_INPUT;
			*i += 1;
			*j += 1;
		}
		else
		{
			prompt->token_status = FAILED;
			prompt->error_msg = SYNTAX_ERROR_NEAR_UNEXPECTED_TOKEN;
			print_error(prompt);
		}
	}
}
