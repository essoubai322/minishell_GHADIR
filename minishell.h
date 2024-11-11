/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:22:04 by amoubine          #+#    #+#             */
/*   Updated: 2024/09/27 11:33:10 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_GLOBAL_H
#define MINISHELL_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>


extern int	g_status;

typedef struct global 
{
    int i;
    char *current_token;
    int current_token_length;
    int in_quote;
    int in_dquote;
    char *error_message;
    char *apa;
    char *str_cmd;
    int k;
    char *stripped_value;
    int o;
    char *env_value;
    char **env;
} g_global;

typedef enum e_type
{
	PIPE = 1,
	RED,
	CMD,
	HEREDOC,
	FILE_N
}					t_type;


enum TokenType {
    WORD,
    PIPE1,
    REDIRECT_IN,
    REDIRECT_OUT,
    REDIRECT_APPEND,
    HEREDOC1,
    QUOTE,
    DQUOTE,
    END
};


typedef struct Token {
    enum TokenType type;
    char *value;
    struct Token *next;
} t_token2;

typedef struct LexerResult {
    t_token2 *tokens;
    char *error_message;
} t_lexer;

typedef struct s_token
{
	char			**args;
	t_type			type;
	int				arg_size;
	struct s_token	*next;
}					t_token;



t_token2 *create_token(enum TokenType type, const char *value);
void add_token(t_token2 **head, enum TokenType type, char *value);
t_lexer tokenize(char *input);
void free_tokens(t_token2 *head);
void print_tokens(t_token2 *head);
void    add_token_v2(t_token2 **head, enum TokenType type, const char *value);



/*global variable*/
extern int	g_status;
/*error.c*/
void		error_func(int errnum, int exit_num);
void		error_exit(char *str, int exit_num);
void		error_and_exit(int exit_num);
void		execve_error(char *cmd, t_token *head, t_list **lists[2],
				char **paths);
/*builtin.c*/
void		echo(char **cmd);
void		print_env(t_list *envl);
int			cd(char **args, t_list **envl, t_list **exp_list, char *path);
char		*pwd(int i, t_list *envl);
void		export(t_list **exp_list, t_list **envl, char *var_name,
				char *var_value);
void		unset(t_list **envl, char *var_name, int flag);
t_list		*setup_env(char **env);
t_list		*setup_exp(t_list *envl);
char		*fenv(t_list *envl, char *str);
char		**convert_to_array(t_list *envl);
void		ft_exit(t_token *head, t_list **lists[2], char **paths, long val);
void		init_export(t_token *head, t_list **envl, t_list **exp_list, int i);
void		change_var_and_free(char *var_name, char *var_value, char *str,
				t_list *tmpl);
int			check_var(char *var_name);
void		print_export(t_list *exp_list);
void		while_print_export(t_list *exp_list);
void		free_v_n(char *v_n, int flag);
/*pipes.c*/
typedef struct s_pipe
{
	int		fd[2];
}			t_pipe;

int			exec_pipes(t_token *list[2], t_list **envl, t_list **exp_list,
				char **paths);
void		init_var_pipe(t_token *head, int i[5]);
void		next_cmd(t_token **head);
void		close_unused_fd(t_pipe *fdt, int p);
void		free_and_wait(t_pipe *fdt, int p, int pid);
int			calc_pipes(t_token *list);
void		ft_exit_pipe(t_token *head, t_list **envl, t_list **exp_list,
				char **paths);
void		clear_child(t_token *head, t_list **envl, t_list **exp_list,
				char **paths);
/*exec.c*/
int			check_pipe(t_token *list);
void		run_cmd(t_token *head, t_list **envl, t_list **exp_list,
				char **paths);
char		*get_path(t_list *envl);
char		**split_paths(char *paths);
int			check_builtin(char *cmd);
void		run(t_token *list[2], t_list **envl, t_list **exp_list,
				char **paths);
char		*check_cmd(char *cmd, char **paths);
int			builtin(t_token *head, t_list **envl, t_list **exp_list);
void		printf_error(char *str, char *cmd, int exit_status);
void		free_run_cmd(char **paths, char **env, char ***arr, int flag);
char		**convert_to_array(t_list *envl);
void		excution(t_token **head, t_list **envl, t_list **exp_list);
void		free_str(char **str1, char **str2);
void		cleanup_run_cmd(char ***env, char **cmd, char **paths,
				t_token *head);
void		setup_lists(t_list **lists[2], t_list **envl, t_list **exp_list);
/*redirection.c*/
int			redir_output(char *filename, int flag);
int			redir_input(char *filename);
int			check_redir(t_token *head, int f);
char		*last_io(t_token *head, int type);
void		redirection(t_token *head, t_list **envl, t_list **exp_list);
void		set_up_env_exp(t_list **envl, t_list **exp_list, char **env);
void		exit_status(int status);
int			check_input_redirection(char *filename);
int			check_while_input_redirection(t_token *list);
void		check_invalid_redirection(t_token *head, t_list **envl,
				t_list **exp_list, char **paths);
void		print_and_exit(char *msg, int status);
void				free_arr(char **arr);
char				*get_var(char *str, t_list *env);
void				signal_setup(int n);
void				list_clear(t_token **head);
void				del(void *c);
int	ft_ncmp(const char *s1, const char *s2, size_t n);
void	error_and_exit(int exit_num);
void	error_func(int errnum, int exit_num);
void	error_exit(char *str, int exit_num);
int	check_builtin(char *cmd);
void	list_clear(t_token **head);
char	*get_path(t_list *envl);
char	**split_paths(char *paths);
char	*ft_get_env(char *name, char **env);

# ifndef DEFAULT_PATH_VALUE
#  define DEFAULT_PATH_VALUE \
	"PATH=/nfs/homes/rbenmakh/.local/bin:\
	/nfs/homes/rbenmakh/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:\
	/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin"
# endif
#endif