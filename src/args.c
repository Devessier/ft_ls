/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 19:29:36 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/13 11:36:45 by bdevessi         ###   ########.fr       */
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

uint8_t	set_group_passwd_link(t_payload *payload, t_uflag flags)
{
	struct passwd	*passwd;
	struct group	*group;
	char			buff[1025];
	ssize_t			len;

	passwd = NULL;
	group = NULL;
	errno = 0;
	if (!(passwd = getpwuid(payload->stats.st_uid)) && errno)
		return (error(payload->d_name, flags));
	if (!(group = getgrgid(payload->stats.st_gid)) && errno)
		return (error(payload->d_name, flags));
	if (!(payload->user = ((flags & FLAG_NUMERIC) || !passwd)
		? ft_itoa(payload->stats.st_uid) : ft_strdup(passwd->pw_name))
		|| !(payload->group = ((flags & FLAG_NUMERIC) || !group)
		? ft_itoa(payload->stats.st_gid) : ft_strdup(group->gr_name)))
		return (1);
	if (S_ISLNK(payload->stats.st_mode))
	{
		if ((len = readlink(payload->d_name, buff, sizeof(buff) - 1)) == -1)
			return (1);
		buff[len] = '\0';
		payload->link = ft_strdup(buff);
	}
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
		if (!(entries->payloads = (t_payload **)malloc(sizeof(t_payload *) * entries->cap)))
		{
			free(tmp);
			return (1);
		}
		i = -1;
		while (entries->payloads && ++i < entries->len)
			entries->payloads[i] = tmp[i];
		if (tmp && entries->len)
			free(tmp);
	}
	if (!(entries->payloads[entries->len] = (t_payload *)malloc(sizeof(t_payload))))
		return (1);
	entries->payloads[entries->len]->stats = stats;
	entries->payloads[entries->len]->d_name = long_name;
	entries->payloads[entries->len++]->d_shname = short_name;
	return ((entries->flags & FLAG_LONG_FORMAT) && set_group_passwd_link(entries->payloads[entries->len - 1], entries->flags) ? error(long_name, entries->flags) : 0);
}
