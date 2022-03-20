/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hashly <hashly@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 01:30:04 by hashly            #+#    #+#             */
/*   Updated: 2022/03/20 14:27:02 by hashly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	malloc_for_export(char *key, char *value, char ***env)
{
	char	**ret;
	size_t	i;
	size_t	j;

	i = 0;
	while ((*env)[i])
		i++;
	ret = (char **)malloc(sizeof(char *) * (i + 2 + 2));
	if (value)
	{
		ret[i] = ft_strjoin(key, "=");
		ret[i] = ft_strjoin_free_s1(ret[i], value);
	}
	else
		ret[i] = ft_strdup(key);
	ret[i + 1] = NULL;
	ret[i + 2] = (*env)[i + 1];
	ret[i + 3] = NULL;
	j = 0;
	while (j++ < i)
		ret[j - 1] = (*env)[j - 1];
	free(*env);
	*env = ret;
}

static void	export_action(char *key, char *value, char ***env)
{
	char	**temp_envp;
	size_t	len_key;
	size_t	i;

	i = 0;
	temp_envp = (*env);
	len_key = ft_strlen(key);
	while (temp_envp[i])
	{
		if (ft_strlen(temp_envp[i]) >= len_key && \
		ft_strncmp(temp_envp[i], key, len_key) == 0 && \
		(temp_envp[i][len_key] == '=' || temp_envp[i][len_key] == '\0'))
		{
			if (value)
			{
				free((*env)[i]);
				(*env)[i] = ft_strjoin(key, "=");
				(*env)[i] = ft_strjoin_free_s1((*env)[i], value);
			}
			return ;
		}
		i++;
	}
	malloc_for_export(key, value, env);
	return ;
}

static int	parsing_argv(char **argv, char ***key, char ***value)
{
	size_t	i;

	i = 0;
	while (argv && argv[i])
	{
		if (argv[i++][0] == '-')
			return (1);
	}
	ft_parsing_argv_2(argv, key, value, 0);
	return (0);
}

static int	no_arguments(char **argv, char **env)
{
	int	i;
	int	len_key;

	if (argv)
		return (1);
	i = 0;
	while (env[i])
	{
		ft_putstr_fd("declare -x ", STD_OUT);
		len_key = 0;
		while (env[i][len_key] && env[i][len_key] != '=')
			len_key++;
		write(STD_OUT, env[i], len_key + 1);
		if (env[i][len_key])
		{
			ft_putstr_fd("\"", STD_OUT);
			ft_putstr_fd(env[i] + len_key + 1, STD_OUT);
			ft_putstr_fd("\"", STD_OUT);
		}
		ft_putstr_fd("\n", STD_OUT);
		i++;
	}
	return (0);
}


/*
нужно проверить на валидность имя переменной окружения

bash: export: `"1abc"=1': not a valid identifier
return (1);
*/
int	ft_export(char **argv, char ***env)
{
	size_t	i;
	char	**key;
	char	**value;

	if (no_arguments(argv, (*env)))
	{
		key = NULL;
		value = NULL;
		if (parsing_argv(argv, &key, &value))
			return (ft_set_ret(1, \
			"minishell: export: invalid argument\n", *env));
		i = 0;
		while (key[i++])
			export_action(key[i - 1], value[i - 1], env);
		i = 0;
		while (key[i])
		{
			free(key[i++]);
			if (value[i - 1])
				free(value[i - 1]);
		}
		free(key);
		free(value);
	}
	return (ft_set_ret(0, NULL, (*env)));
}
