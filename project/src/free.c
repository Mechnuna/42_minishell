/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hashly <hashly@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 22:53:53 by hashly            #+#    #+#             */
/*   Updated: 2022/03/18 15:06:37 by hashly           ###   ########.fr       */
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

/*
Функция для освобождения узла дерева
*/
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
		i = 0;
		if (data->redir)
		{
			while (data->redir[i])
				free(data->redir[i++]);
			free(data->redir);
		}
		free_node_2(data);
	}
	close(node->def_fd[0]);
	close(node->def_fd[1]);
	free(data);
	free(node);
}

/*
Функция для освобождения памяти под дерево
и подмены указателя на env
*/
void	free_forest(t_node *temp, char ***env)
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
