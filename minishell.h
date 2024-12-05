/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 10:22:04 by amoubine          #+#    #+#             */
/*   Updated: 2024/12/05 21:09:55 by asebaai          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <malloc.h>
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

# define BLUE "\001\033[34m\002"
# define PURPLE "\001\033[35m\002"
# define RESET "\001\033[0m\002"
# define BOLD "\001\033[1m\002"

typedef enum e_type
{
	PIPE = 1,
	RED,
	CMD,
	HEREDOC,
	FILE_N
}					t_type;

enum				e_t
{
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

typedef struct Token
{
	enum e_t		type;
	char			*value;
	struct Token	*next;
}					t_token2;

typedef struct LexerResult
{
	t_token2		*tokens;
	char			*error_message;
}					t_lexer;

typedef struct s_token
{
	char			**args;
	t_type			type;
	int				arg_size;
	int				heredoc;
	struct s_token	*next;
}					t_token;

typedef struct s_start
{
	t_token			*current;
	t_token			*prev;
	t_token			*cmd_token;
	t_token			*cmd_prev;
	t_token			*pipe_section;
}					t_start;

typedef struct global
{
	int				red;
	int				i;
	char			*current_token;
	int				current_token_length;
	int				in_quote;
	int				in_dquote;
	char			*error_message;
	char			*apa;
	char			*str_cmd;
	int				k;
	char			*sdv;
	int				o;
	char			*env_value;
	char			**env;
	int				fd_herdoc;
	int				sts;
	t_list			*list[2];
	t_token			*new_head;
	t_token2		*cur;
	char			quote;
	t_token			*tail;
	t_start			start;
	t_token			*tmp;
	t_token			*list0;
	t_token			*list1;
	char			**env_pipe;
}					t_global;

/*token with utils*/

void				while_loop(char *input, t_token2 **head);
t_token2			*create_token(enum e_t type, const char *value);
void				add_token(t_token2 **head, enum e_t type, char *value);
t_lexer				tokenize(char *input);
void				free_tokens(t_token2 *head);
void				print_tokens(t_token2 *head);
void				inialize_global(void);
char				*ft_strcat(char *dest, char *src);
void				ft_free2(char ***dest);
t_token2			*create_token(enum e_t type, const char *value);
void				add_tev(char *env_value, char **stripped_value, char *value,
						int *i);
void				add_token_innit_head(t_token2 **head, char *stripped_value,
						enum e_t type);
int					add_token_check(t_token2 **head, char *stripped_value,
						enum e_t type);
char				*ft_get_value(int *i, char *value);
void				add_token_else(char *value, int *i, char **stripped_value,
						int *k);
char				*get_stripped_value(enum e_t type, char *value,
						char *stripped_value);
void				add_token_v0(char *value, int *i, char **stripped_value,
						int *k);
void				add_token_v1(t_token2 **head, enum e_t type);
void				add_token(t_token2 **head, enum e_t type, char *value);
void				add_token_v2(t_token2 **head, enum e_t type,
						const char *value);
int					counter_closes(char *input);
char				*check_case_v1(char **input, int *i, int *k, char *result);
int					check_case(char *input, int i);
int					expand_variable(const char *input, char *result, int *arr,
						char *env_value);
void				parse_quoted(const char *input, int *i, char *result,
						int *k);
void				parse_unquoted(const char *input, int *i, char *result,
						int *k);
void				parse_unquoted_heredoc1(const char *input, int *i,
						char *result, int *k);
int					parse_double_quote(const char *input, int *i, char *result,
						int *k);
int					parse_single_quote(const char *input, int *i, char *result,
						int *k);
void				skip_whitespace(const char *input, int *i);
char				*string_command(const char *input, int *i);
char				*after_heredoc1(char *input, int *i);
t_lexer				tokenize(char *input);
t_type				check_last_token(t_token *current);
char				*ft_get_env(char *name, char **env);
char				**convert_to_array_v2(t_list *envl, char **global_env);
void				reorganize_cmd_to_start(t_token **head);
t_token				*duplicate_list(t_token *head);
int					initialize_token(t_token **new_token, t_token *current);
void				free_list(t_list *list);
void				*ft_realloc(void *ptr, size_t size);
char				*ft_strcat_stack(char *dest, char *src);
int					while_in_string_command(char *input, t_token2 **head);
void				while_in_current_bigger_than_zero(char *input,
						t_token2 **head);
void				in_expand_variable(char *input, char *var_name,
						char **env_value, int k);
void				while_in_check_red_heredoc(char *input, t_token2 **head);
void				while_in_quote_dquote(char *input, int f);
void				while_in_red_output(char *input, t_token2 **head);
void				while_in_red_input(char *input, t_token2 **head);
void				check_syntax_error_v1(t_token2 *current, char **error);
void				while_in_check_quote_dquote(char *input, t_token2 **head);
void				while_in_expand_variables(char *input);
void				while_in_expand_variable(char *input);
t_token				*convert_data(t_token2 *head, t_token *current2,
						t_token *new_token);
char				*check_syntax_errors(t_token2 *head, char *error_message);
void				check_syntax_error_v3(t_token2 *head, t_token2 *current,
						char **error, int pipe1_count);
void				check_syntax_error_v2(t_token2 *current, char **error);
t_token				*handle_cmd(t_token2 *current, t_token *current2);
int					handle_cmd_args(t_token *current2, const char *value);
t_token				*handle_word_types(t_token2 *current, t_token *new_head);
t_token				*handle_pipe_and_redirects(t_token2 *current);
void				add_token_to_list(t_token **new_head, t_token **current2,
						t_token *new_token);
t_token				*create_and_init_token(const char *value, t_type new_type);
int					add_token_env_value_part(char *env_value, char **std,
						char *value, int i);
int					ft_get_endo(char *value, int i, char **env_value,
						char *var_name);
t_token				*create_and_init_token(const char *value, t_type new_type);
int					unset_errors(char *var_name, int flag);
void				while_redir(t_token *head, int *flag, int r);
char				*handle_token_parsing(char **u, int *i, int *k,
						char *result);
int					handle_quoted_section(char **u, int *i, int *k,
						char *result);
int					handle_unquoted_section(char **u, int *i, int *k,
						char *result);

/*global variable*/

extern t_global		g_glo;
/*error.c*/
void				error_func(int errnum, int exit_num);
void				error_exit(char *str, int exit_num);
void				error_and_exit(int exit_num);
void				execve_error(char *cmd, t_token *head, t_list **lists[2],
						char **paths);
/*builtin.c*/
void				echo(char **cmd);
void				print_env(t_list *envl);
int					cd(char **args, t_list **envl, t_list **exp_list,
						char *path);
char				*pwd(int i, t_list *envl);
void				export(t_list **exp_list, t_list **envl, char *var_name,
						char *var_value);
void				unset(t_list **envl, char *var_name, int flag);
t_list				*setup_env(char **env);
t_list				*setup_exp(t_list *envl);
char				*fenv(t_list *envl, char *str);
char				**convert_to_array(t_list *envl);
void				ft_exit(t_token *head, t_list **lists[2], char **paths,
						long val);
void				init_export(t_token *head, t_list **envl, t_list **exp_list,
						int i);
void				change_var_and_free(char *var_name, char *var_value,
						char *str, t_list *tmpl);
int					check_var(char *var_name);
void				print_export(t_list *exp_list);
void				while_print_export(t_list *exp_list);
void				free_v_n(char *v_n, int flag);
void				unset_v2(t_list **envl, char **var, int flag);
/*pipes.c*/
typedef struct s_pipe
{
	int				fd[2];
}					t_pipe;

int					exec_pipes(t_token *list[2], t_list **envl,
						t_list **exp_list, char **paths);
void				init_var_pipe(t_token *head, int i[5]);
void				next_cmd(t_token **head);
void				close_unused_fd(t_pipe *fdt, int p);
void				free_and_wait(t_pipe *fdt, int p, int pid);
int					calc_pipes(t_token *list);
void				ft_exit_pipe(t_token *head, t_list **envl,
						t_list **exp_list, char **paths);
void				clear_child(t_token *head, t_list **envl, t_list **exp_list,
						char **paths);
int					exec_pipes(t_token *list[2], t_list **envl,
						t_list **exp_list, char **paths);
t_pipe				*init_pipes(int p);

/*exec.c*/
int					check_pipe(t_token *list);
void				run_cmd(t_token *head, t_list **envl, t_list **exp_list,
						char **paths);
char				*get_path(t_list *envl);
char				**split_paths(char *paths);
int					check_builtin(char *cmd);
void				run(t_token *list[2], t_list **envl, t_list **exp_list,
						char **paths);
char				*check_cmd(char *cmd, char **paths);
int					builtin(t_token *head, t_list **envl, t_list **exp_list);
void				printf_error(char *str, char *cmd, int exit_status);
void				free_run_cmd(char **paths, char **env, char ***arr,
						int flag);
char				**convert_to_array(t_list *envl);
void				excution(t_token **head, t_list **envl, t_list **exp_list);
void				free_str(char **str1, char **str2);
void				cleanup_run_cmd(char ***env, char **cmd, char **paths,
						t_token *head);
void				setup_lists(t_list **lists[2], t_list **envl,
						t_list **exp_list);
/*redirection.c*/
int					redir_output(char *filename, int flag);
int					redir_input(char *filename);
int					check_redir(t_token *head, int f);
char				*last_io(t_token *head, int type);
void				redirection(t_token *head, t_list **envl,
						t_list **exp_list);
void				set_up_env_exp(t_list **envl, t_list **exp_list,
						char **env);
void				exit_status(int status);
int					check_input_redirection(char *filename);
int					check_while_input_redirection(t_token *list);
void				check_invalid_redirection(t_token *head, t_list **envl,
						t_list **exp_list, char **paths);
void				print_and_exit(char *msg, int status);
void				free_arr(char **arr);
char				*get_var(char *str, t_list *env);
void				signal_setup(int n);
void				list_clear(t_token **head);
void				del(void *c);
int					ft_ncmp(const char *s1, const char *s2, size_t n);
void				error_and_exit(int exit_num);
void				error_func(int errnum, int exit_num);
void				error_exit(char *str, int exit_num);
int					check_builtin(char *cmd);
char				*get_path(t_list *envl);
char				**split_paths(char *paths);
char				*ft_get_env(char *name, char **env);
void				free_list(t_list *list);
/*heredocs*/
char				*ran_file(void);
int					read_put(char *fn, char *del, int q, t_list *env);
int					heredoc(t_token *head, t_list *list[2], t_token *tmp,
						int status);
void				fork_heredoc(char *fn, t_token *head, t_list *list[2],
						t_token *tmp);
void				free_re(t_token *head, char *file_name);
int					is_q(char *str);
int					ff_strncmp(const char *s1, const char *s2, size_t n);
char				*get_heredoc(char *str, char *del);
void				signal_setup(int n);
void				sig_exit(int num);
void				signal_heredoc(t_list *list[2], t_token *tmp);
char				*rm_quote(char **str);
char				*handle_double_quote(char **str, int *i, char *tmp, int *j);
char				*handle_single_quote(char **str, int *i, char *tmp, int *j);
char				*process_variable(char *s, t_list *env, char *tmp, int *i);
char				*copy_normal_char(char *tmp, char *s, int *i, int *j);
char				*append_var_value(char *tmp, char *var_value, int *i,
						int *j);
char				*var_name_heredoc(char *input, int *i);

# ifndef DEFAULT_PATH_VALUE
#  define DEFAULT_PATH_VALUE \
	"PATH=/home/asebaai/bin:/usr/local/sbin:/usr/local/bin:\
    /usr/sbin:/usr/bin:/sbin:/bin:/usr/games:\
    /usr/local/games:/snap/bin"
# endif
#endif