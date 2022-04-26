/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_symbol.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sstyx <sstyx@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 00:49:13 by sstyx             #+#    #+#             */
/*   Updated: 2022/04/26 23:01:00 by sstyx            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_tokensep(char c)
{
	if (c == '>'
		|| c == '<'
		|| c == '|'
		|| c == ';'
		|| c == '&')
		return (1);
	return (0);
}

int	is_quotes(char c)
{
	if (c == '\''
		|| c == '\"')
		return (1);
	return (0);
}
