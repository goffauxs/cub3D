/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_charset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoffaux <sgoffaux@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 11:16:35 by sgoffaux          #+#    #+#             */
/*   Updated: 2021/10/08 12:10:48 by sgoffaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_table(char **tab, int i)
{
	while (i-- > 0)
		free(tab[i]);
	free(tab);
	return (NULL);
}

static char	*push(char *str, int begin, int end)
{
	int		i;
	char	*temp;

	i = 0;
	temp = malloc(sizeof(char) * (end - begin + 1));
	if (!temp)
		return (NULL);
	while (begin < end)
	{
		temp[i] = str[begin];
		i++;
		begin++;
	}
	temp[i] = '\0';
	return (temp);
}

static int	sep(char c, char *charset)
{
	int	i;

	i = 0;
	while (charset[i] != '\0')
	{
		if (charset[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static int	count_words(char *str, char *charset)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		if (!sep(str[i], charset)
			&& (sep(str[i + 1], charset) || str[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

char	**ft_split_charset(char *str, char *charset)
{
	char	**ret;
	int		begin;
	int		word;
	int		i;

	ret = malloc(sizeof(char *) * (count_words(str, charset) + 1));
	if (!ret)
		return (NULL);
	i = -1;
	word = -1;
	while (str[++i] != '\0')
	{
		if (!sep(str[i], charset)
			&& (i == 0 || sep(str[i - 1], charset)))
			begin = i;
		if ((i != 0 && sep(str[i], charset) && !sep(str[i - 1], charset))
			|| (str[i + 1] == '\0' && !sep(str[i], charset)))
			ret[++word] = push(str, begin, i + (!sep(str[i], charset)));
		if (word != -1 && !ret[word])
			return (free_table(ret, word));
	}
	ret[++word] = 0;
	return (ret);
}
