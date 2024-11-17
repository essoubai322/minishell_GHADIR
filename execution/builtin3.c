/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebaai <asebaai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 09:48:26 by asebaai          #+#    #+#             */
/*   Updated: 2024/09/12 11:46:36 by asebaai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*pwd(int i, t_list *envl)
{
	char	*pwd;
	char	*tmp;

	pwd = getcwd(0, 0);
	if (!pwd)
	{
		tmp = fenv(envl, "PWD");
		pwd = ft_strdup(tmp + 4);
		free(tmp);
	}
	if (i == 1)
	{
		printf("%s\n", pwd);
		free(pwd);
	}
	return (pwd);
}

char	*fenv(t_list *envl, char *str)
{
	while (envl)
	{
		if (ft_strnstr((char *)envl->content, str, ft_strlen(str)))
			return (ft_strdup(envl->content));
		envl = envl->next;
	}
	return (NULL);
}

int	cd_operands(char **args, t_list **envl, t_list **exp_list, char **path)
{
	char	*tmp;

	if (!args[1] || (args[1][0] == '~' && !args[1][1]))
	{
		*path = fenv(*envl, "HOME=");
		if (!*path)
			return (write(2, "cd: HOME not set", 17), 1);
		export(exp_list, envl, "OLDPWD=", tmp = pwd(0, *envl));
		free(tmp);
		chdir(*path + 5);
		return (0);
	}
	else if (args[1][0] == '-')
	{
		*path = fenv(*envl, "OLDPWD=");
		export(exp_list, envl, "OLDPWD=", tmp = pwd(0, *envl));
		free(tmp);
		chdir(*path + 7);
		pwd(1, *envl);
		return (0);
	}
	return (1);
}

void	print_cd_error(void)
{
	write(2, "cd: error retrieving current directory:\n", 41);
	write(2, "getcwd: cannot access parent directories: ", 43);
	write(2, "No such file or directory\n", 27);
}

int	cd(char **args, t_list **envl, t_list **exp_list, char *path)
{
	char	*tmp;

	path = NULL;
	if (args[1] && args[2])
		return (write(2, "cd: string not is pwd\n", 23), 1);
	if (!cd_operands(args, envl, exp_list, &path))
		(void)path;
	else
	{
		export(exp_list, envl, "OLDPWD=", tmp = get_var("PWD", *envl));
		free(tmp);
		tmp = getcwd(NULL, 0);
		if (tmp == NULL)
			print_cd_error();
		free(tmp);
		if (chdir(args[1]))
		{
			printf("cd: %s : ", args[1]);
			error_func(errno, 1);
			return (1);
		}
	}
	export(exp_list, envl, "PWD=", tmp = pwd(0, *envl));
	free(tmp);
	return (free(path), 0);
}
