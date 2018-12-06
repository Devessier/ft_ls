/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 19:29:36 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/06 10:21:19 by bdevessi         ###   ########.fr       */
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

	errno = 0;
	if ((watch_sym_link ? lstat : stat)(long_name, &stats) != 0)
		return (error(long_name));
	entries = S_ISDIR(stats.st_mode) ? dir_entries : files_entries;
	if (entries->len + 1 >= entries->cap)
	{
		tmp = entries->payloads;
		entries->cap = !entries->cap ? 10 : entries->cap * 2;
		errno = 0;
		if (!(entries->payloads = (t_payload **)malloc(sizeof(t_payload *) * entries->cap)))
			error(long_name);
		i = -1;
		while (entries->payloads && ++i < entries->len)
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
