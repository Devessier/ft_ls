/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 19:29:36 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/02 12:19:20 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "list.h"
#include <errno.h>
#include "utils.h"
#include "sort.h"

t_argument	g_arguments[] =
{
	{ 'l', FLAG_LONG_FORMAT },
	{ 'R', FLAG_RECURSIVE },
	{ 'a', FLAG_INCLUDE_DOTS },
	{ 'r', FLAG_REVERSE_SORT },
	{ 't', FLAG_SORT_TIME_MODIFIED },
	{ 0, FLAG_NONE }
};

int		normalize_argument(char **str)
{
	int		i;

	i = 0;
	while ((*str)[i])
		i++;
	if (i)
		i--;
	while (i && (*str)[i] == '/')
		(*str)[i--] = '\0';
	return (i);
}

uint8_t		parse_flags(char *flag)
{
	uint8_t	flags;
	uint8_t	i;

	flags = FLAG_NONE;
	while (*++flag)
	{
		i = 0;
		while (g_arguments[i].c_flag)
			if (g_arguments[i++].c_flag == *flag)
				flags |= g_arguments[i - 1].flag;
	}
	return (flags);
}

int		append_entry(t_entries *entries, char *long_name, char *short_name)
{
	t_stat	**tmp;
	int	i;

	if (entries->len + 1 >= entries->cap)
	{
		tmp = entries->stats;
		entries->cap = !entries->cap ? 10 : entries->cap * 10;
		if (!(entries->stats = (t_stat **)malloc(sizeof(t_stat *) * entries->cap)))
			error(long_name);
		i = -1;
		while (++i < entries->len)
			entries->stats[i] = tmp[i];
		if (tmp && entries->len)
			free(tmp);
	}
	errno = 0;
	if (!(entries->stats[entries->len] = (t_stat *)malloc(sizeof(t_stat)))
		|| stat(long_name, (struct stat *)entries->stats[entries->len]) != 0)
		return (error(long_name));
	entries->stats[entries->len]->d_name = long_name;
	entries->stats[entries->len++]->d_shname = short_name;
	return (0);
}

t_entries	parse_args(int len, char **args)
{
	uint8_t		end_of_flags;
	t_entries	arguments;
	int		i;

	arguments = (t_entries){ 0, 0, 0, NULL };
	end_of_flags = 0;
	i = 0;
	while (i < len && *args[i] == '-' && !end_of_flags)
	{
		if (args[i][1] == '-')
			end_of_flags ^= 1;
		else
			arguments.flags |= parse_flags(args[i]);
		i++;
	}
	if (!(len - i))
		append_entry(&arguments, ".", ".");
	else if (len - i > 1)
		quick_sort((void **)args, i, len - 1, ft_strcmp);
	while (i < len)
	{
		normalize_argument(&args[i]);
		append_entry(&arguments, args[i], args[i]);
		i++;
	}
	return (arguments);
}
