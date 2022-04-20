/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a79856 <a79856@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 17:39:26 by gcredibl          #+#    #+#             */
/*   Updated: 2022/04/20 03:11:14 by a79856           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 
#include "../../inc/minishell.h"

void	process_syserror(void)
{
	char	*msg;

	msg = strerror(errno);
	write(2, "Error: ", 7);
	write(2, msg, ft_strlen(msg));
	exit(errno);
}

void	process_error(void)
{
	char	*msg;

	msg = strerror(errno);
	write(2, "Error: ", 7);
	write(2, msg, ft_strlen(msg));
}

int	process_input_error(size_t errcode)
{
	char	*msg[50];

	msg[1] = "No matching quote\n";
	msg[2] = "syntax error near unexpected token `<`\n";
	msg[3] = "syntax error near unexpected token `|`\n";
	msg[4] = "syntax error near unexpected token `;`\n";
	msg[5] = "syntax error near the redirect token\n";
	write(2, "minishell: ", 11);
	write(2, msg[errcode], ft_strlen(msg[errcode]));
	return (-1);
}

int	perr(char *cmd, char *arg, char *err, int code)
{
	ft_putstr_fd("minishell", 2);
	ft_putstr_fd(": ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	if (err)
		ft_putstr_fd(err, 2);
	ft_putstr_fd("\n", 2);
	return (code);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int		i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}