/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epresa-c <epresa-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:01:49 by Emiliano          #+#    #+#             */
/*   Updated: 2022/07/12 16:42:22 by epresa-c         ###   ########.fr       */
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
# include <errno.h>
# include <limits.h>
# include <sys/ioctl.h>

# define FALSE 0
# define TRUE 1

# define CLOSED 0
# define OPEN 1

# define NO_INIT 0

# define FAILED -1

# define NO_ERROR 0
# define MALLOC_ERROR -42
# define ERROR_TOKEN -44
# define ERROR_SYNTAX_PIPE_AT_START -46
# define SYNTAX_ERROR_NEAR_UNEXPECTED_TOKEN -48

# define UNFINISHED 0
# define FINISHED 1

# define CHAR_NOT_FOUND -1

# define REDIR_INPUT 1
# define REDIR_OUTPUT_SIMPLE 2
# define REDIR_OUTPUT_APPEND 3
# define HERE_DOC 4

# define EXECUTABLE 1
# define NON_EXECUTABLE -1

int	g_exit_status;

/* ***************************** STRUCTURES ********************************* */

typedef struct s_count_words
{
	int	q_type;
	int	q_status;
	int	count;
	int	i;
}	t_count_words;

typedef struct s_quote_parsing
{
	int		s_len;
	int		q_simple;
	int		q_double;
	int		i;
	int		str_idx;
	int		tab_index;
}	t_quote_parsing;

typedef struct s_cmd
{
	char			**full_cmd;
	char			*full_path;
	int				infile;
	int				outfile;
	int				is_builtin;
	char			*status;
	int				exec_stat;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_prompt
{
	t_cmd	*cmds;
	char	**envp;
	int		**pipes;
	pid_t	*pid;
	char	**paths;
	char	*prompt_text;
	int		n_cmds;
	int		stop;
	int		error_msg;
	int		token_status;
}			t_prompt;

typedef struct s_var
{
	char	*line;
	char	**split;
	char	**tmp;
	char	**s_split;
	int		i;
	int		j;
}	t_var;

/* *****************************  main.c  ********************************** */

void	init_mini_vars(t_var *v, t_prompt *prompt, char **envp);

/* *************************** node_management ***************************** */

void	ft_new_prompt(int sig);

/* ****************************** signals.c ******************************** */

void	signal_handler1(int sig);
void	signal_handler2(int sig);
void	signals_default(void);
void	signals_at_start(void);

/* *****************************  split.c  ********************************** */

void	init_count_words_struct(t_count_words *w);
int		ft_count_words(const char *str, char *caracter);
char	**ft_split_str_with_spaces_and_quotes(char const *s, t_prompt *prompt);
void	ft_fill_split(char **splited, char const *str, char *caracter);
void	init_quote_parsing_struct(t_quote_parsing *q, char const *str);

/* ****************************  s_split.c  ******************************** */

void	fn_sub_split(t_var *v);
char	**ft_cmds_split(char const *s, char *set);

/* *****************************  lexer.c  ********************************** */

void	free_all_tabs_and_prompt(t_var *v, t_prompt *prompt);
void	fn_lexer(t_var *v, t_prompt *prompt);
void	print_error(t_prompt *prompt);

/* **************************  utils_lexer.c  ******************************* */

char	*get_prompt(char **envp);
void	init_t_var_main(t_var *v);
void	init_t_prompt(t_prompt *prompt, char **envp);
void	print_tab_with_str_name(char **tab, char *tab_name);

/* *****************************  expander.c  ******************************* */

void	fn_expander(t_var *v, t_prompt *prompt);
int		search_charset_index_inside_str(char *str, char *set);
char	*get_var_name(char *str, t_prompt *prompt);
char	*get_var_name_dquotes(char *str, t_prompt *prompt, int *idx_after);
char	*expand_vars(char *s_split_i, t_prompt *prompt);

/* ***************************  expander_utils.c  *************************** */

void	update_quote_status(char *s_split_i, t_quote_parsing *q);
char	*expand_path(char *s_split_i, char *str_home);
void	pars_expand_status(char *s_split_i, int *expand_status);
char	**cd_expantion_home(t_cmd *curr, char **envp);

/* **************************  parsing.c  *********************************** */

void	fn_parsing(t_var *v, t_prompt *prompt);

/* ***********************  node_t_cmd_create.c  **************************** */

t_cmd	*start_t_cmd(t_prompt *prompt);
void	add_t_cmd(t_prompt *prompt);
int		count_cmds(t_prompt *prompt);

/* ***********************  node_t_cmd_fill.c  **************************** */

void	fill_t_cmd(t_var *v, t_prompt *prompt, int k);
void	fill_cmd_with_redir(t_var *v, int *i, int redir_status, t_cmd *curr, int *open_redir_status);

/* **********************  check_builtin_and_redir.c  *********************** */

int		check_builtin(char *cmd);
void	is_builtin_is_exit(t_cmd *curr, t_prompt *prompt, int i, t_var *v);
void	is_redir(t_var *v, int *i, int *j, int *redir_status, t_prompt *prompt);

/* ******************************  error.c  ********************************* */

void	fn_echo_error(t_cmd *curr, char *subplit_i, char *err_msg);

/* ******************************  print_list.c  **************************** */

void	print_list(t_prompt *prompt);

/* ******************************  var_utils.c  ***************************** */

int		env_var_exist(char *name, char **envp);
char	*get_env(char *name, char **my_envp);
char	**set_env(char *name, char *var, t_prompt *s_pr);

/* *****************************  var_builtin.c  **************************** */

char	**export_builtin(t_cmd *cmd, t_prompt *s_pr);
char	**unset_builtin(char *name, t_prompt *s_pr);
void	env_builtin(t_prompt *s_pr);
void	echo_builtin(t_cmd *cmd);

/* *****************************  utils_tab.c  ****************************** */

int		tablen(char **t);
char	**tab_add(char **src, char *add);
void	tab_free(char **tab);
void	print_tab(char **tab);
char	**tab_delone(char **src, char **dest, int to_del);

/* *****************************  open_files.c  *************************** */

char	*here_input(char *limiter, int fd);
int		open_in_files(char *in_file, char *limiter, t_cmd *cmd);
int		open_outfiles(char *out_file, int append, t_cmd *cmd);

/* *****************************  mini_builtin.c  *************************** */

int		pwd_builtin(char **my_envp, int fd);
int		cd_builtin(t_cmd *cmd, t_prompt *s_pr);

/* *****************************  init_data.c  *************************** */

char	**init_envp(char **envp);
void	init_path(t_prompt *s_p);
char	*create_path(char **paths, char *cmdn);

/* *****************************  pipes_pid.c  *************************** */

void	tab_int_free(int **tab);
void	create_pipes_pids(t_prompt *s_pr);
void	close_pipes(t_prompt *s_pr);

/* *******************************  exec.c  ****************************** */

void	wait_status(int exitstatus);
void	exec_cmd(t_prompt *s_pr, t_cmd *cur_cmd);
void	prep_exec(t_prompt *s_pr, t_cmd *cur_cmd, int num);

/* **************************** utils_tab.c ****************************** */

void	print_tab(char **tab);

/* **************************** prepare_exec.c **************************** */

int		search_function(t_prompt *s_pr, t_cmd *cur_cmd, int num);
int		builtin_is_redir(t_prompt *s_pr, t_cmd *cur_cmd, int num);
int		read_list(t_prompt *s_pr);

/* ************************* prep_builtin_exec.c ************************* */

int		search_builtin(t_prompt *s_pr, t_cmd *cur, int num);
void	redir_builtin(t_prompt *s_pr, t_cmd *cur_cmd, int num);
int		fork_builtin(t_prompt *s_pr, t_cmd *cur_cmd, int num);
void	builtin_close_redir(t_cmd *cur_cmd);
int		builtin_is_redir(t_prompt *s_pr, t_cmd *cur_cmd, int num);

/* ******************************** test  ********************************* */

void	single_child_cmd(t_cmd *cmd, t_prompt *s_p);
void	child_cmd1(t_cmd *cmd, t_prompt *s_p);
void	prep_child2(t_cmd *curr, t_prompt *s_p);
int		test_complete_cmd(t_prompt *s_pr, t_cmd *cur_cmd, int num);

/* ******************************** termios  ****************************** */

void	echo_ctrl_on(void);
void	echo_ctrl_off(void);

#endif
