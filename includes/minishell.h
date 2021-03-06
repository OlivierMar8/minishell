/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epresa-c <epresa-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:01:49 by Emiliano          #+#    #+#             */
/*   Updated: 2022/06/17 15:10:47 by epresa-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "../libft/get_next_line.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <signal.h>
# include <fcntl.h>
# include <unistd.h>
# include <dirent.h>
# include <stddef.h>
# include <readline/history.h>
# include <termios.h>

# define FALSE 0
# define TRUE 1

# define CLOSED 0
# define OPEN 1

# define NO_INIT 0

int	g_exit_status;

/* ***************************** STRUCTURES ********************************* */

typedef struct s_count_words
{
	int	q_type;
	int	q_status;
	int	count;
	int	i;
}	t_count_words;

typedef struct s_fill_array
{
	int		s_len;
	int		quote_simple;
	int		quote_double;
	int		i;
	int		str_idx;
	int		tab_index;
}	t_fill_array;

typedef struct s_cmd
{
	char			**full_cmd;
	char			*full_path;
	int				infile;
	int				outfile;
	int				is_builtin;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_prompt
{
	t_cmd	*cmds;
	char	**envp;
	int		**pipes;
	pid_t	*pid;
	char	*prompt_text;
	int		n_cmds;
}			t_prompt;

typedef struct s_var
{
	char	*line;
	char	**split;
	char	**tmp;
	char	**subsplit;
	int		i;
	int		j;
}	t_var;

/* *****************************  main.c  ********************************** */



/* *************************** node_management ***************************** */

// void	start_stack(t_node **tail, t_node **head, int value);
// void	insert_beginning_stack(t_node **tail, int value);
// void	add_command_to_list(t_node **head, int value);

void	ft_new_prompt(int sig);

/* ****************************** signals.c ******************************** */

void	signal_handler(int sig);

/* *****************************  split.c  ********************************* */

void	init_count_words_struct(t_count_words *w);
int		ft_count_words(const char *str, char *caracter);
char	**ft_split_str_with_spaces_and_quotes(char const *s);
void	ft_fill_array(char **splited, char const *str, char *caracter);

/* ****************************  subsplit.c  ******************************* */

void	fn_sub_split(t_var *v);
char	**ft_cmdsubsplit(char const *s, char *set);

/* *****************************  exit.c  *********************************** */

void	ft_exit(int exit_status);

/* *****************************  lexer.c  *********************************** */

void	free_all_tabs(t_var *v);
void	fn_lexer(t_var *v, t_prompt *prompt);

/* *****************************  expander.c  ******************************** */

void	fn_expander(t_var *v);

/* **************************  utils_lexer.c  ******************************* */

char	*get_prompt(char **envp);
void	init_t_var_main(t_var *v);
void	init_t_prompt(t_prompt *prompt, char **envp);
void	print_tab_with_str_name(char **tab, char *tab_name);

/* *****************************  BRANCHE o_pipe  *************************** */

int		mini_pwd(char **my_envp);
char	*here_input(char *limiter);
void	fd_putendl_fd(char *s, int fd);
int		tablen(char **t);
char	**tab_add(char **src, char *add);
int		env_var_exist(char *name, char **envp);
int		export_builtin(t_cmd *cmd, char **envp);
char	**init_envp(char **envp);
void	ft_print_tab_model(char **temp);
char	*get_env(char *name, char **my_envp);
int		set_env(char *name, char *var, char **my_env);
int		unset_builtin(char *name, char **my_envp);
void	tab_free(char **tab);
int		mini_cd(t_cmd *cmd, char **my_env);

/* ******************************** utils_tab.c ****************************** */

void	print_tab(char **tab);

#endif
