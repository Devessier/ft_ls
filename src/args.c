/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 19:29:36 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/10 22:23:19 by bdevessi         ###   ########.fr       */
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
	{ 'G', FLAG_COLORS_ON },
	{ 'U', FLAG_CREATION },
	{ 'R', FLAG_RECURSIVE },
	{ 'a', FLAG_INCLUDE_DOTS },
	{ 'c', FLAG_STATUS_CHANGED },
	{ 'l', FLAG_LONG_FORMAT },
	{ 'n', FLAG_NUMERIC },
	{ 'r', FLAG_REVERSE_SORT },
	{ 't', FLAG_SORT_TIME_MODIFIED },
	{ 'u', FLAG_LAST_ACCESS },
	{ 0, FLAG_NONE }
};

uint8_t	set_group_passwd(t_payload *payload, t_uflag flags)
{
	struct passwd	*passwd;
	struct group	*group;

	passwd = NULL;
	group = NULL;
	errno = 0;
	if (!(passwd = getpwuid(payload->stats.st_uid)) && errno)
		return (error(payload->d_name));
	errno = 0;
	if (!(group = getgrgid(payload->stats.st_gid)) && errno)
		return (error(payload->d_name));
	if (!(payload->user = ((flags & FLAG_NUMERIC) || !passwd)
		? ft_itoa(payload->stats.st_uid) : ft_strdup(passwd->pw_name)))
		return (1);
	if (!(payload->group = ((flags & FLAG_NUMERIC) || !group)
		? ft_itoa(payload->stats.st_gid) : ft_strdup(group->gr_name)))
		return (1);
	return (0);
}

int		append_entry(t_entries *entries, struct stat stats,
	char *long_name, char *short_name)
{
	t_payload		**tmp;
	int				i;

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
	if ((entries->flags & FLAG_LONG_FORMAT) && set_group_passwd(entries->payloads[entries->len - 1], entries->flags))
		return (error(long_name));
	return (0);
}
