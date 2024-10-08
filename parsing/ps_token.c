/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarabul <nkarabul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:59:13 by nkarabul          #+#    #+#             */
/*   Updated: 2024/09/13 17:29:02 by nkarabul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	quote_check(char *str)
{
	int		i;
	int		q_count;
	char	c;

	q_count = 0;
	i = 0;
	while (str[i])
	{
		if ((str[i] == '\"' || str[i] == '\'') && q_count % 2 == 0)
		{
			c = str[i];
			q_count += 1;
		}
		else if (str[i] == c)
			q_count++;
		i++;
	}
	if (q_count % 2 != 0)
	{
		printf("QUOTE ERROR\n");
		free(str);
	}
}

char	*quote_remover(char *str, int i, int j)
{
	char	*removed;
	int		strsize;

	strsize = ft_strlen(str);
	removed = malloc(sizeof(char) * strsize + 1);
	if(!removed)
		return (NULL);
	while (str[i])
	{
		while (str[i] == DOUBLEQ || str[i] == SINGLEQ)
			i++;
		removed[j] = str[i];
		j++;
		i++;
	}
	removed[j] = '\0';
	free(str);
	return (removed);
}

void	tokenize1(char *str, char *org_str, int i)
{
	while (str[i])
	{
		if (str[i] == '"')
		{
			str[i] = DOUBLEQ;
			org_str[i] = str[i];
			i++;
			while (str[i] != '"')
				str[i++] = CHAR;
			str[i] = DOUBLEQ;
			org_str[i] = str[i];
		}
		else if (str[i] == '\'')
		{
			str[i] = SINGLEQ;
			org_str[i] = str[i];
			i++;
			while (str[i] != '\'')
				str[i++] = CHAR;
			str[i] = SINGLEQ;
			org_str[i] = str[i];
		}
		i++;
	}
}

void	tokenize2(char *str, char *org_str, int i) // str free
{
	while (str[i])
	{
		if (str[i] == '>' && str[i + 1] == '>')
		{
			org_str[i] = APPEND;
			org_str[i + 1] = APPEND;
			i += 2;
			continue;
		}
		else if (str[i] == '<' && str[i + 1] == '<')
		{
			org_str[i] = HEREDOC;
			org_str[i + 1] = HEREDOC;
			i += 2;
			continue;
		}
		if (str[i] == '>')
			org_str[i] = OUTPUT;
		else if (str[i] == '<')
			org_str[i] = INPUT;
		else if (str[i] == '|')
			org_str[i] = PIPE;
		i++;
	}
}
