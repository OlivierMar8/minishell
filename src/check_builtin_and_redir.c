/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtin_and_redir.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epresa-c <epresa-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 10:25:09 by epresa-c          #+#    #+#             */
/*   Updated: 2022/07/06 15:20:04 by epresa-c         ###   ########.fr       */
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
			if (v->subsplit[i + 1] && ft_atoi(v->subsplit[i + 1]) <= INT_MAX \
				&& ft_atoi(v->subsplit[i + 1]) >= INT_MIN)
				g_exit_status = ft_atoi(v->subsplit[i + 1]);
		}
		curr->exec_stat = NON_EXECUTABLE;
	}
	else
		prompt->stop = FALSE;
}

void	is_redir(t_var *v, int *i, int *j, int *redir_status, t_prompt *prompt)
{
	if (v->subsplit[*i][0] == '>' && v->subsplit[*i + 1] && v->subsplit[*i + 1][0] == '>')
	{
		if (v->subsplit[*i + 2] != NULL && ft_strchr("~%^{}:; |\\", v->subsplit[*i + 2][0]) == 0)
		{
			*redir_status = REDIR_OUTPUT_APPEND;
			printf("\tdetect REDIR_OUTPUT_APPEND at i = %d\n", *i);
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
	if (v->subsplit[*i][0] == '<' && v->subsplit[*i + 1] && v->subsplit[*i + 1][0] == '<')
	{
		if (v->subsplit[*i + 2] != NULL && ft_strchr("~%^{}:; |\\", v->subsplit[*i + 2][0]) == 0)
		{
			*redir_status = HERE_DOC;
			printf("\tdetect HERE_DOC at i = %d\n", *i);
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
	if (v->subsplit[*i][0] == '>')
	{
		if (v->subsplit[*i + 1] != NULL && ft_strchr("~%^{}:; |\\", v->subsplit[*i + 1][0]) == 0)
		{
			*redir_status = REDIR_OUTPUT_SIMPLE;
			printf("\tdetect REDIR_OUTPUT_SIMPLE at i = %d\n", *i);
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
	if (v->subsplit[*i][0] == '<')
	{
		if (v->subsplit[*i + 1] != NULL && ft_strchr("~%^{}:; |\\", v->subsplit[*i + 1][0]) == 0)
		{
			*redir_status = REDIR_INPUT;
			printf("\tdetect REDIR_INPUT at i = %d\n", *i);
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
