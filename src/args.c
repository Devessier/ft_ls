/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 19:29:36 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/03 14:48:35 by bdevessi         ###   ########.fr       */
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
#include <unistd.h>

t_argument	g_arguments[] =
{
	{ 'l', FLAG_LONG_FORMAT },
	{ 'R', FLAG_RECURSIVE },
	{ 'a', FLAG_INCLUDE_DOTS },
	{ 'r', FLAG_REVERSE_SORT },
	{ 't', FLAG_SORT_TIME_MODIFIED },
	{ 'G', FLAG_COLORS_ON },
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
	if ((flags & FLAG_COLORS_ON) && !isatty(1))
		flags ^= FLAG_COLORS_ON;
	return (flags);
}

int		append_entry(t_entries *entries, char *long_name, char *short_name, uint8_t watch_sym_link)
{
	t_stat	**tmp;
	int		i;
	int		(*stat_fn)(const char *path, struct stat *buf);

	stat_fn = watch_sym_link ? lstat : stat;
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
		|| stat_fn(long_name, (struct stat *)entries->stats[entries->len]) != 0)
		return (error(long_name));
	entries->stats[entries->len]->d_name = long_name;
	entries->stats[entries->len++]->d_shname = short_name;
	return (0);
}

t_entries	parse_args(int len, char **args)
{
	uint8_t		end_of_flags;
	t_entries	arguments;
	int			i;
	int			j;

	arguments = (t_entries) { isatty(1) ? FLAG_COLORS_ON : 0, 0, 0, NULL };
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
		append_entry(&arguments, ".", ".", 0);
	else if (len - i > 1)
		quick_sort((void **)args, i, len - 1, ft_strcmp);
	j = i;
	while (i < len)
	{
		normalize_argument(&args[i]);
		append_entry(&arguments, args[i], args[i], 0);
		i++;
	}
	if (len - j > 1)
		quick_sort((void **)arguments.stats, j, len - 1, args_sort);
	return (arguments);
}
