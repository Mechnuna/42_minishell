/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a79856 <a79856@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 22:12:09 by hashly            #+#    #+#             */
/*   Updated: 2022/04/04 17:05:50 by a79856           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
Функция для получения строки из терминала с помощью readline
promt - начальная строка aka bash
*/
static char	*get_line(char ***env)
{
	char	*line_read;
	char	*promt;

	promt = get_promt(*env);
	line_read = readline(promt);
	free(promt);
	if (line_read && *line_read)
		add_history(line_read);
	if (!line_read)
	{
		ft_free_envp(&env);
		sig_d(0);
	}
	return (line_read);
}

void	ft_parse_split(t_parser *prs)
{
	if (prs->str && prs->str[0])
		prs->mass = ft_add_line(prs->mass, prs->str);
	free(prs->str);
	prs->str = NULL;
}

char	*parce(char *str, t_parser *prs)
{
	int	i;

	i = 0;
	prs->str = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '\'')
			str = ft_gap(str, &i, prs);
		else if (str[i] == '\\')
			str = ft_slash(str, &i, prs);
		else if (str[i] == '\"')
			str = ft_quotechar(str, &i, prs);
		else if (str[i] == '$')
			str = ft_dollar(str, &i, prs);
		else if (str[i] == '>' || str[i] == '<' || str[i] == '|'
			|| str[i] == '&' || str[i] == '*' || str[i] == ';' || str[i] == '('
			|| str[i] == ')')
			str = ft_replace(str, &i, str[i], prs);
		else if (str[i] == ' ' || str[i] == '\t' || str[i] == '>' || str[i] == '<')
		{
			if (prs->red == '>' || prs->red == '<')
				prs->str = ft_charjoin(prs->str, str[i]);
			else
				ft_parse_split(prs);
		}
		else
		{
			prs->str = ft_charjoin(prs->str, str[i]);
			prs->red = 0;
		}
		i++;
	}
	ft_parse_split(prs);
	return (str);
}

/*
Masha
Эта функция разбивает строку str на составные части. В строках должна быть
информация следующего рода :
	cmd
	option or argv
	>	file_name
	>>	file_name
	<	file_name
	<<	file_name
	(
	)
	&&
	||
	|
	;
	${NAME}
	NULL

Т.к. аргумент может быть '(' или '< file_name' и т.д. Следует записать для
ключевых токенов особенные записи, чтобы в последующем не перепутать токены
и аргументы к команде. Поэтому в итоге нужно, чтобы
строка строк была в следующем виде:
	cmd
	option or argv
	\1\2>\3\23	file_name
	\1\2>>\3\23	file_name
	\1\2<\3\23	file_name
	\1\2<<\3\23	file_name
	\1\2(\3\23
	\1\2)\3\23
	\1\2&&\3\23
	\1\2||\3\23
	\1\2|\3\23
	\1\2;\3\23
	\001\002${NAME}\3\23
	NULL
*/
static char	**split_str(char *str, char **env)
{
	t_parser	*prs;
	char		**ret;

	prs = malloc(sizeof(t_parser));
	// prs->mass = (char **)malloc(sizeof(char *) * (1));
	prs->quo = 0;
	prs->red = 0;
	prs->d_quo = 0;
	prs->mass = NULL;
	if (!env)
		;
	str = parce(str, prs);
	free(str);
	free(prs->str);
	ret = prs->mass;
	free (prs);
	return (ret);
}

/* функция для посчета кавычек */

int	preparse(char *str)
{
	char	c;
	int		i;
	int		c2;
	int		c3;
	int		dollar;

	i = 0;
	c = '0';
	c2 = 0;
	c3 = 0;
	dollar = 0;
	while (str[i])
	{
		if ((str[i] == '\'') || str[i] == '"' || str[i] == '\\')
		{
			if (!(i != 0 && str[i - 1] == '\\'))
			{
				if (c == str[i])
					c = '0';
				else if (c == '0')
					c = str[i];
			}
		}
		else if (str[i] == '(' || str[i] == ')' || str[i] == '{' || str[i] == '}')
		{
			if (str[i] == '(' && c == '0')
				c2++;
			else if (str[i] == ')' && c == '0')
				c2--;
			else if (str[i] == '{' && c == '0')
				c3++;
			else if (str[i] == '}' && c == '0')
				c3--;
		}
		i++;
	}
	if (c != '0' || (c2 + c3) != 0)
		return (0);
	return (1);
}

char	*lexer(char *str)
{
	char	*error;
	int		i;
	int		red;
	char	q;

	i = 0;
	error = NULL;
	red = 0;
	q = '0';
	while (str[i] != '\0')
	{
		while (ft_strchr(";& \r\v\n\t|", str[i]) != NULL && q == '0')
		{
			if (ft_strchr(";&|", str[i]))
			{
				if (red == 0)
				{
					if (((error != NULL && error[0] == str[i])
					|| !(error)) && ft_strlen(error) != 2)
						error = ft_charjoin(error, str[i]);
				}
				else
					red = 0;
			}
			i++;
		}
		if (str[i] == '\"' || str[i] == '\'')
		{
			if (str[i] == q)
				q = '0';
			else
				q = str[i];
		}
		red++;
		i++;
	}
	if (error != NULL && str[i] == '\0')
		return (ft_strjoin_free_s2(SYN_ERR, ft_charjoin(error, '\n')));
	return (NULL);
}

char	*check_redirect(char *str)
{
	int		i;
	int		red;
	char	q;
	int		start;
	int		finish;

	i = 0;
	red = 0;
	start = 0;
	finish = 0;
	q = '0';
	while (str[i] != '\0')
	{	
		if (start > 0 && red > 0 && finish > 0)
		{
			red = 0;
			start = 0;
			finish = 0;
		}
		while (ft_strchr(">< \r\v\n\t", str[i]) != NULL && q == '0' && str[i] != '\0')
		{
			if (ft_strchr("<>", str[i]))
				red++;
			i++;
		}
		if (str[i] == '\"' || str[i] == '\'')
		{
			if (str[i] == q)
				q = '0';
			else
				q = str[i];
		}
		if (str[i] == '\0')
			break;
		if (red == 0)
			start++;
		else
			finish++;
		i++;
	}
	if (red > 0 && ((red == 0 && finish == 0) || finish == 0))
		return ("syntax error near unexpected token `newline'\n");
	return (NULL);
}

/*
Функция для чтения с стандартного ввода команды с помощью readline, а затем
разбиения этой строки на составные части
*/
char	**parsing(char ***env)
{
	char	*str;
	char	**ret;
	char	*error;

	ret = NULL;
	str = get_line(env);
	//Здесь ошибочно считается ошибкой и кейс, вроде `)`
	//Вызывает вывод текста "minishell: syntax error: unexpected end of file"
	//А нужно выводить "bash: syntax error near unexpected token `)'"
	//
	//Еще ошибочно проверяется доллар (вернее он здесь не должен проверяться)
	//Если я все правильно понял в парсере, то здесь должны быть проверены
	//только кавычки
	//Хотя тебе лучше знать. может здесь стоит проверить и еще на какие-то ошибки парсера
	if (!(preparse(str)))
	{
		free(str);
		ft_set_ret(2, PROGRAM_NAME": syntax error: unexpected end of file\n", *env);
		return (NULL);
	}
	error = lexer(str);
	if (error != NULL)
	{
		free(str);
		ft_set_ret(2, error, *env);
		return (NULL);
	}
	else {
		error = check_redirect(str);
		if (error != NULL)
		{
			free(str);
			ft_set_ret(2, error, *env);
			return (NULL);
		}
	}
	if (str[0] == 0)
	{
		ret = ft_add_line(ret, str);
		free(str);
	}
	else
		ret = split_str(str, *env);
	return (ret);
}
