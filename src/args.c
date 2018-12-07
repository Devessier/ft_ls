/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 19:29:36 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/07 14:48:38 by bdevessi         ###   ########.fr       */
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
#include <pwd.h>
#include <grp.h>

t_argument	g_arguments[] =
{
	{ 'l', FLAG_LONG_FORMAT },
	{ 'R', FLAG_RECURSIVE },
	{ 'a', FLAG_INCLUDE_DOTS },
	{ 'r', FLAG_REVERSE_SORT },
	{ 't', FLAG_SORT_TIME_MODIFIED },
	{ 'G', FLAG_COLORS_ON },
	{ 'n', FLAG_NUMERIC },
	{ 0, FLAG_NONE }
};

uint8_t	set_group_passwd(t_payload *payload)
{
	struct passwd	*passwd;
	struct group	*group;

	errno = 0;
	if (!(passwd = getpwuid(payload->stats.st_uid)) && errno)
		return (1);
	errno = 0;
	if (!(group = getgrgid(payload->stats.st_gid)) && errno)
		return (1);
	payload->passwd = *passwd;
	payload->group = *group;
	return (0);
}

int		append_entry(t_entries *dir_entries, t_entries *files_entries, char *long_name, char *short_name, uint8_t watch_sym_link)
{
	(void)watch_sym_link;
	t_payload		**tmp;
	int				i;
	struct stat		stats;
	t_entries		*entries;


	errno = 0;
	if (lstat(long_name, &stats) != 0)
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
	if ((entries->flags & FLAG_LONG_FORMAT) && set_group_passwd(entries->payloads[entries->len - 1]))
		return (error(long_name));
	return (0);
}
