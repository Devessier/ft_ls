/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 19:29:36 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/20 10:10:45 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/acl.h>
#include <sys/xattr.h>
#include "ft_ls.h"

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

void	set_extd_attr_acl(t_payload *payload)
{
	acl_t		acl;
	ssize_t		extd_attr_len;
	acl_entry_t	entry;

	acl = NULL;
	extd_attr_len = listxattr(payload->d_name, NULL, 0, XATTR_NOFOLLOW);
	acl = acl_get_link_np(payload->d_name, ACL_TYPE_EXTENDED);
	if (acl && acl_get_entry(acl, ACL_FIRST_ENTRY, &entry) == -1)
		acl = NULL;
	if (acl)
		acl_free(acl);
	payload->has_acl = !!acl;
	payload->has_ea = extd_attr_len > 0;
}

uint8_t	set_group_passwd_link(t_payload *payload, t_uflag flags)
{
	const struct passwd	*passwd = getpwuid(payload->stats.st_uid);
	const struct group	*group = getgrgid(payload->stats.st_gid);
	char				buff[1025];
	ssize_t				len;

	errno = 0;
	if (!(passwd && group) && errno)
		return (error(payload->d_name, flags));
	if (!(payload->group = ((flags & FLAG_NUMERIC) || !group)
		? ft_itoa(payload->stats.st_gid) : ft_strdup(group->gr_name)))
		return (1);
	if (!(payload->user = ((flags & FLAG_NUMERIC) || !passwd)
		? ft_itoa(payload->stats.st_uid) : ft_strdup(passwd->pw_name)))
		return (free_grp_usr(payload->group, NULL));
	if (S_ISLNK(payload->stats.st_mode))
	{
		if ((len = readlink(payload->d_name, buff, sizeof(buff) - 1)) == -1)
			return (free_grp_usr(payload->group, payload->user));
		buff[len] = '\0';
		if (!(payload->link = ft_strdup(buff)))
			return (free_grp_usr(payload->group, payload->user));
	}
	return (0);
}

uint8_t	set_long_format_data(t_payload *payload, t_uflag flags)
{
	set_extd_attr_acl(payload);
	errno = 0;
	return (set_group_passwd_link(payload, flags));
}

void	set_payload(t_payload *payload, struct stat stats,
	char *d_name, char *d_shname)
{
	payload->stats = stats;
	payload->d_name = d_name;
	payload->d_shname = d_shname;
}

int		append_entry(t_entries *entries, struct stat stats,
	char *l_n, char *s_h)
{
	t_payload		**tmp;
	int				i;

	if (entries->len + 1 >= entries->cap)
	{
		tmp = entries->payloads;
		entries->cap = !entries->cap ? 10 : entries->cap * 2;
		if (!(entries->payloads = malloc(sizeof(t_payload *) * entries->cap)))
		{
			entries->payloads = tmp;
			return (1);
		}
		i = -1;
		while (entries->payloads && ++i < entries->len)
			entries->payloads[i] = tmp[i];
		if (tmp && entries->len)
			free(tmp);
	}
	if (!(entries->payloads[entries->len] = malloc(sizeof(t_payload))))
		return (1);
	set_payload(entries->payloads[entries->len++], stats, l_n, s_h);
	return (entries->flags & FLAG_LONG_FORMAT && set_long_format_data(
		entries->payloads[entries->len - 1], entries->flags)
		? error(l_n, entries->flags) : 0);
}
