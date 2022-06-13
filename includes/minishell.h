/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Emiliano <Emiliano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:01:49 by Emiliano          #+#    #+#             */
/*   Updated: 2022/06/10 18:32:04 by Emiliano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include <stddef.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include "../libft/libft.h"
# include "../libft/get_next_line.h"

# define FALSE 0
# define TRUE 1

# define CLOSED 0
# define OPEN 1

# define NO_INIT 0

/* ***************************** STRUCTURES ********************************* */

int exit_status;

typedef struct s_count_words
{
    int quote_type;
    int quote_status;
    int counter;
} t_count_words;

typedef struct s_fill_array
{
	int		s_len;
    int     quote_simple;
    int     quote_double;
    int     i;
    int     start_index;
    int     tab_index;    
}           t_fill_array;

typedef struct s_cmd
{
	char            *full_cmd;
	char            *full_path;
	int             infile;
	int	            outfile;
    struct s_cmd    *next;
    struct s_cmd    *prev;
}			        t_cmd;

typedef struct s_prompt
{
	t_cmd  *cmds;
	char	**envp;
	pid_t	pid;
    char    *prompt_text;
}			t_prompt;

typedef struct s_var
{
    char    *line;
    char    **split;
    char    **subsplit;
}           t_var;

/* *****************************  main.c  ********************************* */

char    *get_prompt(void);

/* *************************** node_management ***************************** */

// void	start_stack(t_node **tail, t_node **head, int value);
// void	insert_beginning_stack(t_node **tail, int value);
// void	add_command_to_list(t_node **head, int value);

void    ft_new_prompt(int sig);
void    sig_quit(int sig);

/* *****************************  split.c  ********************************* */

void    init_count_words_struct(t_count_words *w);
int    ft_count_words(const char *str, char *caracter);
char	**ft_split_str_with_spaces_and_quotes(char const *s);
void    ft_fill_array(char **splited, char const *str, char *caracter);

/* ****************************  subsplit.c  ******************************* */

void    fn_sub_split(t_var *v);
char	**ft_cmdsubsplit(char const *s, char *set);

/* *****************************  exit.c  *********************************** */
void    ft_exit(int exit_status);

#endif