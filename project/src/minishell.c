/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hashly <hashly@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 19:45:08 by hashly            #+#    #+#             */
/*   Updated: 2022/02/12 17:55:50 by hashly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	**cmd_line;
	t_node	*root;

	g_envp = ft_copy_env(envp);
	argv = NULL;
	if (argc != 1)
		return (0);
	set_signal();
	while (1)
	{
		cmd_line = parsing(); //Masha
		root = get_forest(cmd_line); //ILYA
		free_cmd_line(&cmd_line); //+
		if (node_is_not_empty(root))
			execute(root); //ILYA
		free_forest(root); //ILYA
	}
	return (0);
}
