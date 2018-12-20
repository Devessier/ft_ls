/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 10:57:31 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/14 11:24:25 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>
#include "ft_ls.h"

t_file_type	g_file_types[] = {
	{ S_IFIFO, COLOR_FIFO, 'p' },
	{ S_IFCHR, COLOR_CHR, 'c' },
	{ S_IFBLK, COLOR_BLK, 'b' },
	{ S_IFDIR, COLOR_DIR, 'd' },
	{ S_IFLNK, COLOR_LNK, 'l' },
	{ S_IFSOCK, COLOR_SOCK, 's' },
	{ S_IFREG, "", '-' },
	{ 0, NULL, 0 }
};

void	list_file(t_payload *payload, t_uflag flags, t_maxs *maximums)
{
	if (!(flags & FLAG_LONG_FORMAT))
		print_color_file(payload, flags);
	else
		long_format(payload, flags, maximums);
}

void	calculate_max_len(t_maxs *maximums)
{
	maximums->links_len = nb_len(maximums->links);
	maximums->size_len = nb_len(maximums->size);
	maximums->major_len = nb_len(maximums->major);
	maximums->minor_len = nb_len(maximums->minor);
}

void	list_files(const t_entries *entries, t_maxs *maxs)
{
	int	i;

	i = 0;
	while (i < entries->len)
		list_file(entries->payloads[i++], entries->flags, maxs);
}

void	list_dir(t_payload *stats, t_uflag flags, uint8_t print_name)
{
	const t_entries	e = (t_entries) { flags, 0, 0, 0 };
	int				i;
	t_maxs			m;

	m = (t_maxs) { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	if (print_name)
		ft_putf_fd(1, "\n%s:\n", stats->d_name);
	if (read_directory(&e, stats, flags, &m))
		return (free_entries((t_entries*)&e, flags));
	calculate_max_len(&m);
	sort_entries((void**)e.payloads, 0, e.len - 1, flags);
	if (flags & FLAG_LONG_FORMAT && e.len)
		ft_putf_color_fd(1, "\033[4;34m", e.flags, "total %d\n", m.blocks);
	list_files(&e, &m);
	i = 0;
	while ((flags & FLAG_RECURSIVE) && i < e.len)
	{
		if (S_ISDIR(e.payloads[i]->stats.st_mode)
			&& ft_strcmp(".", e.payloads[i]->d_shname, flags) &&
			ft_strcmp("..", e.payloads[i]->d_shname, flags))
			list_dir(e.payloads[i], flags, 1);
		i++;
	}
	free_entries((t_entries*)&e, flags);
}

void	list_argument(t_payload *argstat, t_uflag flags)
{
	if (!argstat->stats.st_mode)
		return ;
	if (S_ISDIR(argstat->stats.st_mode))
		list_dir(argstat, flags, 0);
	else
		list_file(argstat, flags, NULL);
}
