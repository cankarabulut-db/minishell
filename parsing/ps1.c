#include "../minishell.h"

void rdr_makezero(t_rdr *count)
{
	count->acwhile = 0;
	count->icwhile = 0;
	count->hcwhile = 0;
	count->ocwhile = 0;
	count->oc = 0;
	count->ic = 0;
	count->ac = 0;
	count->hc = 0;
}

void pipe_redirect_ba(char *str,int i)
{
	while(str[i])
	{
		if(str[i] == PIPE)
		{
			i++;
			if(str[i] == 0)
				error_msg(str,PIPE);
			while((str[i] >= '\t' && str[i] <= 13) || str[i] == ' ')
				i++;
			if(str[i] == 0)
				error_msg(str,PIPE);
		}
		i++;
	}
}

void heredoc_append_control(char *str,int i)
{
	while(str[i])
	{
		if((str[i] == HEREDOC && str[i + 1] == HEREDOC) || \
		(str[i] == APPEND && str[i+1] == APPEND))
		{
			i+=2;
			if(str[i] == 0)
				error_msg(str,1);
			while((str[i] >= '\t' && str[i] <= 13) || str[i] == ' ')
				i++;
			if(str[i] == 0)
				error_msg(str,1);
		}
		i++;
	}
}

void input_output_control(char *str,int i)
{
	while(str[i])
	{
		if(str[i] == INPUT || str[i] == OUTPUT)
		{
			i++;
			if(str[i] == 0)
				error_msg(str,1);
			while((str[i] >= '\t' && str[i] <= 13) || str[i] == ' ')
				i++;
			if(str[i] == 0)
				error_msg(str,1);
		}
		i++;
	}
}
int redirect_counter(char *str,int i,int redirectType)
{
	int count;

	count = 0;
	while(str[i])
	{
		if(str[i] == redirectType && str[i+1] == redirectType)
		{
			count++;
			i+=2;
		}
		else if(str[i] == redirectType)
		{
			count++;
			i++;
		}
		i++;
	}
	return (count);
}
void empty_maker(char *str,char a,int start,int len)
{
	int i = 0;

	i = start;
	if(str[start] == DOUBLEQ || str[start] == SINGLEQ)
	{
		start++;
		while(len + i > start && str[start] && \
		(str[start] != DOUBLEQ && str[start] != SINGLEQ))
		{
			str[start] = a;
			start++;
		}
	}
	else
	{
		while(len + i > start && str[start] && (str[start] != ' ' && str[start] != '\t'))
		{
			str[start] = a;
			start++;
		}
	}
	
}
void redirects_filler(t_shell *cmd,char *str,t_rdr *count,int i)
{
	int start;

	start = i;

	if(str[i] == DOUBLEQ || str[i] == SINGLEQ)
	{
		i++;
		while((str[i] != DOUBLEQ && str[i] != SINGLEQ) && str[i])
			i++;
	}
	else
	{
		while((str[i] != ' ' && str[i] != '\t') && str[i])
			i++;
	}
		
	if(count->ic > count->icwhile)
		cmd->input[count->icwhile++] = ft_substr(str,start,i - start);
	else if(count->oc > count->ocwhile)
		cmd->output[count->ocwhile++] = ft_substr(str,start,i - start);
	else if(count->ac > count->acwhile)
		cmd->append[count->acwhile++] = ft_substr(str,start,i - start);
	else if(count->hc > count->hcwhile)
		cmd->heredoc[count->hcwhile++] = ft_substr(str,start,i - start);
	empty_maker(str,' ',start,i - start);
	printf("%s\n",str);
}

void redirect_malloc(t_shell *cmd,char *str,t_rdr *rdrc)
{
	rdrc->ic = redirect_counter(str,0,INPUT);
	rdrc->oc = redirect_counter(str,0,OUTPUT);
	rdrc->ac = redirect_counter(str,0,APPEND);
	rdrc->hc = redirect_counter(str,0,HEREDOC);

	cmd->input = malloc(sizeof(char *) * rdrc->ic + 1);
	cmd->output = malloc(sizeof(char *) * rdrc->oc+ 1);
	cmd->heredoc = malloc(sizeof(char *) * rdrc->hc + 1);
	cmd->append = malloc(sizeof(char *) * rdrc->ac + 1);
}
void redirect_find_fill(t_shell *cmd,char *str,int i,int type)
{
	t_rdr rdrCount;

	rdr_makezero(&rdrCount);
	redirect_malloc(cmd,str,&rdrCount);
	while(str[i])
	{
		if(str[i] == INPUT || str[i] == OUTPUT || \
			str[i] == APPEND || str[i] == HEREDOC)
		{
			if(type == APPEND || type == HEREDOC)
			{
				str[i] = ' ';
				str[i+1] = ' ';
				i+=2;
			}
			else
			{
				str[i] = ' ';
				i++;
			}
			while(str[i] == '\t' || str[i] == ' ')
				i++;
			redirects_filler(cmd,str,&rdrCount,i);
		}
		i++;
	}
}

void split_pipe(t_shell *cmd,char *str,int i, int size)
{
	char **pipe_cmd;
	t_shell *temp;

	pipe_cmd = ft_split(str,PIPE);
	size = ft_strplen(pipe_cmd);
	temp = cmd;
	while(size > i)
	{
		heredoc_append_control(pipe_cmd[i],0);
		input_output_control(pipe_cmd[i],0);
		redirect_find_fill(cmd,pipe_cmd[i],0,0);
		cmd->next = malloc(sizeof(t_shell));
		cmd = cmd->next;
		i++;
	}
	cmd = temp;
}
void struct_filler(t_shell *cmd, char *str, int i)
{
	cmd = malloc(sizeof(t_shell));
	(void)i;
		if(ft_exist(str,PIPE,0))
			split_pipe(cmd,str,0,0);
}
