/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epresa-c <epresa-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 15:01:32 by epresa-c          #+#    #+#             */
/*   Updated: 2022/07/04 15:34:57 by epresa-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	check_quotes_and_delete(t_prompt *prompt, t_var *v, int i) //check if it's possible to increment k at other place
{
	char			*aux;
	int				k;
	t_quote_parsing	q;

	init_quote_parsing_struct(&q, NULL);
	k = -1;
	aux = malloc((ft_strlen(v->subsplit[i]) + 1) * sizeof(char));
	if (aux == NULL)
	{
		prompt->error_msg = MALLOC_ERROR;
		prompt->token_status = FAILED;
		return ;
	}
	while (v->subsplit[i][q.i] != '\0')
	{
		update_quote_status(v->subsplit[i], &q);
		if ((v->subsplit[i][q.i] != '\"' || q.quote_simple) && \
		(v->subsplit[i][q.i] != '\'' || q.quote_double) && ++k >= 0)
			aux[k] = v->subsplit[i][q.i];
		q.i++;
	}
	aux[++k] = '\0';
	free(v->subsplit[i]);
	v->subsplit[i] = aux;
}

void	print_error(t_prompt *prompt)
{
	if (prompt->error_msg == MALLOC_ERROR)
		ft_printf("minishell: malloc error\n");
	if (prompt->error_msg == ERROR_TOKEN)
		ft_printf("Error token\n");
	if (prompt->error_msg == ERROR_SYNTAX_PIPE_AT_START)
		ft_printf("minishell: syntax error near unexpected token \'|\'\n");
	if (prompt->error_msg == SYNTAX_ERROR_NEAR_UNEXPECTED_TOKEN)
		ft_printf("minishell: syntax error\n");
	prompt->error_msg = NO_ERROR;
}

void	fn_delete_quotes(t_var *v, t_prompt *prompt)
{
	int	i;

	i = 0;
	while (v->subsplit[i] != NULL)
	{
		check_quotes_and_delete(prompt, v, i);
		if (prompt->error_msg != NO_ERROR)
		{
			print_error(prompt);
			return ;
		}
		i++;
	}
}

void	print_error_token(t_var *v)
{
	ft_printf("Error token\n");
	free(v->line);
	v->line = NULL;
}

void	fn_lexer(t_var *v, t_prompt *prompt)
{
	int	ret;

	ret = 0;
	prompt->token_status = TRUE;
	v->split = ft_split_str_with_spaces_and_quotes(v->line, prompt); //this functions return NULL if ther's a quote inconsistence <<USE the new print_error function
	if (v->split == NULL)
	{
		print_error_token(v);
		return ;
	}
	else
	{
		v->i = 0;
		while (v->split[v->i])
		{
			v->tmp = ft_cmdsubsplit(v->split[v->i], "<|>");
			v->j = 0;
			while (v->tmp[v->j] != NULL)
			{
				v->subsplit = tab_add(v->subsplit, v->tmp[v->j]);
				v->j++;
			}
			tab_free(v->tmp);
			free(v->tmp);
			v->tmp = NULL;
			v->i++;
		}
		if (v->subsplit[0][0] == '|')
		{
			prompt->error_msg = ERROR_SYNTAX_PIPE_AT_START;
			print_error(prompt);
			prompt->token_status = FAILED;
		}
		fn_expander(v, prompt);
		fn_delete_quotes(v, prompt);
		init_path(prompt);
	}
}
