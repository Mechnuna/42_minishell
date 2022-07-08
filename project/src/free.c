/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a79856 <a79856@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 22:53:53 by hashly            #+#    #+#             */
/*   Updated: 2022/07/08 04:06:53 by a79856           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	free_node_2(t_data *data)
{
	if (data->cmd)
		free(data->cmd);
	if (data->cmd_exec)
		free(data->cmd_exec);
}

void	free_node(t_node *node)
{
	size_t	i;
	t_data	*data;

	i = 0;
	data = node->data;
	if (!node->next_lvl)
	{
		if (data->argv)
		{
			while (data->argv[i])
				free(data->argv[i++]);
			free(data->argv);
		}
		free_node_2(data);
	}
	if (node->list_redir)
		ft_lstclear(&node->list_redir, &free_node_list_redir);
	close(node->def_fd[0]);
	close(node->def_fd[1]);
	free(data);
	free(node);
}

void	free_forest(t_node *temp, char ****env)
{
	t_node	*for_free;

	*env = temp->env;
	while (temp->next_lvl || temp->prev_lvl || temp->next)
	{
		for_free = temp;
		if (temp->next_lvl)
		{
			temp = temp->next_lvl;
			for_free->next_lvl = NULL;
		}
		else if (temp->next)
		{
			temp = temp->next;
			free_node(for_free);
		}
		else if (temp->prev_lvl)
		{
			temp = temp->prev_lvl;
			free_node(for_free);
		}
	}
	free_node(temp);
}
