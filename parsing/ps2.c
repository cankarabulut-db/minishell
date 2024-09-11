/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarabul <nkarabul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 13:43:20 by nkarabul          #+#    #+#             */
/*   Updated: 2024/09/11 21:16:37 by nkarabul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	redirect_malloc(t_shell *cmd, char *str, t_rdr *rdrc)
{
	rdrc->ic = redirect_counter(str, 0, INPUT);
	rdrc->oc = redirect_counter(str, 0, OUTPUT);
	rdrc->ac = redirect_counter(str, 0, APPEND);
	rdrc->hc = redirect_counter(str, 0, HEREDOC);
	cmd->input = malloc(sizeof(char *) * rdrc->ic + 1);
	cmd->output = malloc(sizeof(char *) * rdrc->oc + 1);
	cmd->heredoc = malloc(sizeof(char *) * rdrc->hc + 1);
	cmd->append = malloc(sizeof(char *) * rdrc->ac + 1);
}

void	redirect_find_fill(t_shell *cmd, char *str, int i, t_rdr *rdrcount)
{
	rdr_makezero(rdrcount);
	redirect_malloc(cmd, str, rdrcount);
	while (str[i])
	{
		if (str[i] == INPUT || str[i] == OUTPUT || \
			str[i] == APPEND || str[i] == HEREDOC)
		{
			rdrcount->type = str[i];
			if (rdrcount->type == APPEND || rdrcount->type == HEREDOC)
			{
				str[i] = ' ';
				str[i + 1] = ' ';
				i += 2;
			}
			else
				str[i++] = ' ';
			while (str[i] == '\t' || str[i] == ' ')
				i++;
			redirects_filler(cmd, str, rdrcount, i);
		}
		i++;
	}
}

void cmd_find_fill(t_shell *cmd,char *str,int i)
{
	int start;

	while(str[i] == '\t' || str[i] == ' ')
		i++;
	start = i;
	while((str[i] != ' ' && str[i] != '\t') && str[i])
		i++;
	cmd->cmd = quote_remover(ft_substr(str , start, i - start),0,0);;
	empty_maker(str,' ',start,i - start);
}

void	split_pipe_and_fill(t_shell *cmd, char *str, int i, t_rdr *listsize)
{
	char	**pipe_cmd;
	t_shell	*temp;

	pipe_cmd = ft_split(str, PIPE);
	listsize->listsize = ft_strplen(pipe_cmd);
	temp = cmd;
	while (listsize->listsize > i)
	{
		input_output_control(pipe_cmd[i], 0);
		heredoc_append_control(pipe_cmd[i], 0);
		redirect_find_fill(cmd, pipe_cmd[i], 0, listsize);
		cmd_find_fill(cmd,pipe_cmd[i],0);
		pipe_cmd[i] = quote_remover(pipe_cmd[i],0,0);
		cmd->args = ft_split(pipe_cmd[i],' ');
		make_empty(pipe_cmd[i],-1);
		printf("Environment : %s\n",cmd->main_env[0]);
		cmd->next = malloc(sizeof(t_shell));
		cmd->next = cmd->main_env;
		cmd = cmd->next;
		i++;
	}
	cmd = temp;
}

void	struct_filler(t_shell *cmd, char *str, int i)
{
	t_rdr	list;

	cmd = malloc(sizeof(t_shell));
	(void)i;
	if (ft_exist(str, PIPE, 0))
		split_pipe_and_fill(cmd, str, 0, &list);
}
