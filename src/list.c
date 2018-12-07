/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 10:57:31 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/07 15:21:00 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "utils.h"
#include "args.h"
#include "sort.h"
#include "list.h"
#include <sys/stat.h>

t_file_type	g_file_types[] = {
	{ S_IFIFO, COLOR_FIFO, 'p' },
	{ S_IFCHR, COLOR_CHR, 'c' },
	{ S_IFBLK, COLOR_BLK , 'b' },
	{ S_IFDIR, COLOR_DIR, 'd' },
	{ S_IFLNK, COLOR_LNK, 'l' },
	{ S_IFSOCK, COLOR_SOCK, 's' },
	{ S_IFREG, "" , '-' },
	{ 0, NULL, 0 }
};

int		error(char *path)
{
	ft_putf_fd(2, "ft_ls: %s: %s\n", path, strerror(errno))	;
	return (1);
}

char	*color_code(t_payload *payload, uint8_t flags)
{
	const mode_t	st_mode = payload->stats.st_mode;
	uint8_t			i;

	if (!(flags & FLAG_COLORS_ON))
		return ("");
	i = 0;
	while (g_file_types[i].mode)
		if ((st_mode & S_IFMT) == g_file_types[i++].mode
				&& *g_file_types[i - 1].color)
			return (g_file_types[i - 1].color);
	if (st_mode & S_IXUSR)
		return (COLOR_EXEC);
	return ("");
}

void	print_file_type(mode_t perms, uint8_t flags)
{
	uint8_t	i;

	i = 0;
	while (g_file_types[i].mode)
		if ((perms & S_IFMT) == g_file_types[i++].mode)
			ft_putf_fd(1, "%s%c%s",
					flags & FLAG_COLORS_ON ? COLOR_FILE_TYPE : "",
					g_file_types[i - 1].to_char,
					flags & FLAG_COLORS_ON ? COLOR_RESET : "");
}

void	print_file_mode(mode_t perms, uint8_t flags)
{
	int8_t	shift;

	print_file_type(perms, flags);
	shift = 9;
	while ((shift -= 3) >= 0)
	{
		ft_putf_fd(1, "%s%c%s", flags & FLAG_COLORS_ON ? COLOR_READ : "", (perms & (4 << shift)) ? 'r' : '-', flags & FLAG_COLORS_ON ? COLOR_RESET : "");
		ft_putf_fd(1, "%s%c%s", flags & FLAG_COLORS_ON ? COLOR_WRITE : "", (perms & (2 << shift)) ? 'w' : '-', flags & FLAG_COLORS_ON ? COLOR_RESET : "");
		if (shift && ((!(perms & S_IXUSR) && (perms & S_ISUID))
				|| (!(perms & S_IXGRP) && (perms & S_ISGID))))
			ft_putchar_fd('S', 1);
		else if (shift && (((perms & S_IXUSR) && (perms & S_ISUID))
				|| ((perms & S_IXGRP) && (perms & S_ISGID))))
			ft_putchar_fd('s', 1);
		else
		{
			if (!shift && perms & S_ISVTX)
				ft_putchar_fd(perms & (1 << shift) ? 't' : 'T', 1);
			else
				ft_putf_fd(1, "%s%c%s", flags & FLAG_COLORS_ON ? COLOR_EXEC : "", (perms & (1 << shift)) ? 'x' : '-', flags & FLAG_COLORS_ON ? COLOR_RESET : "");
		}
	}
}

void	long_format(t_payload *payload, uint8_t flags, t_maxs *maximums)
{
	(void)maximums;
	//printf("%d, %d, %d, %d, %d\n", maximums->links, maximums->user, maximums->group, maximums->size, maximums->blocks);
	print_file_mode(payload->stats.st_mode, flags);
	ft_putchar_fd('\n', 1);
}

void	list_file(t_payload *payload, uint8_t flags, t_maxs *maximums)
{
	if (!(flags & FLAG_LONG_FORMAT))
		ft_putf_fd(1, "%s%s%s\n", color_code(payload, flags), payload->d_shname, (flags & FLAG_COLORS_ON) ? COLOR_RESET : "");
	else
		long_format(payload, flags, maximums);
}

void	free_stats(t_payload *stats)
{
	free(stats->d_shname);
	free(stats->d_name);
	free(stats);
}

void	set_longer_string(unsigned int *size, char *str)
{
	const size_t	len = ft_strlen(str);

	if (*size < len)
		*size = len;
}

void	list_dir(t_payload *stats, uint8_t flags, uint8_t print_name)
{
	DIR				*directory;
	t_entries		entries;
	struct dirent	*d;
	int				i;
	char			*short_name;
	t_maxs			maximums;

	maximums = (t_maxs) { 0, 0, 0, 0, 0 };
	if (print_name)
		ft_putf_fd(1, "\n%s:\n", stats->d_name);
	entries = (t_entries){ flags, 0, 0, NULL };
	errno = 0;
	if (!(directory = opendir(stats->d_name)))
		return ((void)error(stats->d_name));
	i = 0;
	while ((d = readdir(directory)) != NULL)
	{
		if (*d->d_name == '.' && !(flags & FLAG_INCLUDE_DOTS))
			continue ;
		if (append_entry(&entries, &entries, pathjoin(stats->d_name, d->d_name), strdup(d->d_name), 1))
			return ;
		if (flags & FLAG_LONG_FORMAT)
		{
			if (entries.payloads[i]->stats.st_nlink > maximums.links)
				maximums.links = entries.payloads[i]->stats.st_nlink;
			if (entries.payloads[i]->stats.st_size > maximums.size)
				maximums.size = entries.payloads[i]->stats.st_size;
			set_longer_string(&maximums.user, entries.payloads[i]->passwd.pw_name);
			set_longer_string(&maximums.group, entries.payloads[i]->group.gr_name);
			maximums.blocks += entries.payloads[i++]->stats.st_blocks;
		}
	}
	if (entries.len > 1)
		quick_sort((void **)entries.payloads , 0, entries.len - 1, ft_d_name_sort, flags);
	i = 0;
	while (i < entries.len)
		list_file(entries.payloads[i++], entries.flags, &maximums);
	i = 0;
	while ((flags & FLAG_RECURSIVE) && i < entries.len)
	{
		short_name = entries.payloads[i]->d_shname;
		if (S_ISDIR(entries.payloads[i++]->stats.st_mode))
			if (ft_strcmp(".", short_name) && ft_strcmp("..", short_name))
				list_dir(entries.payloads[i - 1], flags, 1);
		free_stats(entries.payloads[i - 1]);
	}
	closedir(directory);
	free(entries.payloads);
}

void	list_argument(t_payload *argstat, uint8_t flags)
{
	if (!argstat->stats.st_mode)
		return ;
	if (S_ISDIR(argstat->stats.st_mode))
		list_dir(argstat, flags, 0);
	else
		list_file(argstat, flags, NULL);
}
