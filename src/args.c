/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 19:29:36 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/03 17:52:24 by bdevessi         ###   ########.fr       */
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

int		append_entry(t_entries *dir_entries, t_entries *files_entries, char *long_name, char *short_name, uint8_t watch_sym_link)
{
	t_payload		**tmp;
	int				i;
	struct stat		stats;
	t_entries		*entries;	

	if ((watch_sym_link ? lstat : stat)(long_name, &stats) != 0)
		return (error(long_name));
	entries = S_ISDIR(stats.st_mode) ? dir_entries : files_entries;
	if (entries->len + 1 >= entries->cap)
	{
		tmp = entries->payloads;
		entries->cap = !entries->cap ? 10 : entries->cap * 10;
		if (!(entries->payloads = (t_payload **)malloc(sizeof(t_payload *) * entries->cap)))
			error(long_name);
		i = -1;
		while (++i < entries->len)
			entries->payloads[i] = tmp[i];
		if (tmp && entries->len)
			free(tmp);
	}
	errno = 0;
	if (!(entries->payloads[entries->len] = (t_payload *)malloc(sizeof(t_payload))))
		return (error(long_name));
	entries->payloads[entries->len]->stats = stats;
	entries->payloads[entries->len]->d_name = long_name;
	entries->payloads[entries->len++]->d_shname = short_name;
	return (0);
}

/*void		parse_args(int len, char **args)
{
	uint8_t		end_of_flags;
	t_entries	files_arguments;
	t_entries	dir_arguments;
	int			i;

	files_arguments = (t_entries) { isatty(1) ? FLAG_COLORS_ON : 0, 0, 0, NULL };
	dir_arguments = (t_entries) { isatty(1) ? FLAG_COLORS_ON : 0, 0, 0, NULL };
	end_of_flags = 0;
	i = 0;
	while (i < len && *args[i] == '-' && !end_of_flags)
	{
		if (args[i][1] == '-')
			end_of_flags ^= 1;
		else
			files_arguments.flags |= parse_flags(args[i]);
		i++;
	}
	dir_arguments.flags = files_arguments.flags;
	if (!(len - i))
		append_entry(&dir_arguments, NULL, ".", ".", 0);
	i = 0;
	while (i < len)
	{
		normalize_argument(&args[i]);
		append_entry(&files_arguments, &dir_arguments, args[i], args[i], 0);
		i++;
	}
	quick_sort((void **)files_arguments.stats, 0, files_arguments.len, args_sort, files_arguments.flags);
	quick_sort((void **)dir_arguments.stats, 0, dir_arguments.len, args_sort, dir_arguments.flags);
	print_entry(&files_arguments);
	print_entry(&dir_arguments);
}*/
