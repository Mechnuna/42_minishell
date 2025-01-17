/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a79856 <a79856@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 10:56:00 by hashly            #+#    #+#             */
/*   Updated: 2022/07/08 04:06:46 by a79856           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	**get_argv(t_node *node)
{
	char	**ret;
	int		i;

	i = 0;
	ret = NULL;
	ret = ft_add_line(ret, node->data->cmd);
	while (node->data->argv && node->data->argv[i])
	{
		ret = ft_add_line(ret, node->data->argv[i]);
		i++;
	}
	return (ret);
}

static void	ft_exec_child(t_node *node)
{
	char	**argv;

	open_path_and_check_access(node);
	argv = get_argv(node);
	execve(node->data->cmd_exec, argv, *node->env);
	ft_free_str_of_str(&argv);
	if (errno == ENOEXEC)
		exit(0);
	ft_putstr_fd(PROGRAM_NAME": ", STD_ERR);
	perror(node->data->cmd);
	exit(errno);
}

static void	ft_execve_node(t_node *node)
{
	pid_t	pid;
	int		ret;

	node->data->cmd_exec = ft_strdup(node->data->cmd);
	pid = fork();
	if (pid == 0)
		ft_exec_child(node);
	else
	{
		if (waitpid(pid, &ret, 0) == -1)
			return (perror("WAIT_PID"));
		if (WIFSIGNALED(ret))
			ft_set_ret(130, NULL, *node->env);
		else if (WIFEXITED(ret))
			ft_set_ret(WEXITSTATUS(ret), NULL, *node->env);
	}
	return ;
}

static void	ft_exec_next_lvl(t_node *node)
{
	pid_t	pid;
	int		ret;

	pid = fork();
	if (pid == 0)
	{
		up_shlvl(node->env);
		execute_level(node->next_lvl);
		exit(ft_atoi(ft_get_status(*node->env)));
	}
	else
	{
		waitpid(pid, &ret, 0);
		if (WIFSIGNALED(ret))
			ft_set_ret(130, NULL, *node->env);
		else if (WIFEXITED(ret))
			ft_set_ret(WEXITSTATUS(ret), NULL, *node->env);
	}
}

void	execute_cmd_in_node(t_node *node)
{
	if (node->next_lvl && cond_status(node) == 0)
		ft_exec_next_lvl(node);
	else if (!node->next_lvl)
	{
		if (node->exec == 1)
			return ;
		node->exec = 1;
		if (!node->data->cmd && node->exec != 1)
			node->stop = 1;
		if (node->exit == 1 || !node->data->cmd || cond_status(node))
			return ;
		preparsing(node);
		if (node->stop)
			return ;
		if (cond_is_built_in(node))
			;
		else if (node->data->cmd[0] != 0)
			ft_execve_node(node);
	}
}
