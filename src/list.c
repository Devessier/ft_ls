/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 10:57:31 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/10 15:08:31 by bdevessi         ###   ########.fr       */
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
#include "utils.h"
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

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
	if (st_mode & S_ISUID && st_mode & S_AEXEC)
		return (COLOR_UID);
	if (st_mode & S_ISGID && st_mode & S_AEXEC)
		return (COLOR_GID);
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
			ft_putstr_color_fd(g_file_types[i - 1].to_char,
					COLOR_FILE_TYPE, 1, flags);
}

void	print_file_mode(mode_t perms, uint8_t flags)
{
	int8_t	sh;

	print_file_type(perms, flags);
	sh = 9;
	while ((sh -= 3) >= 0)
	{
		ft_putstr_color_fd((perms & 4 << sh) ? 'r' : '-', COLOR_READ, 1, flags);
		ft_putstr_color_fd((perms & 2 << sh) ? 'w' : '-', COLOR_WRITE, 1, flags);
		if (sh && ((sh == 6 && !(perms & S_IXUSR) && perms & S_ISUID)
				|| (sh == 3 && !(perms & S_IXGRP) && perms & S_ISGID)))
			ft_putstr_color_fd('S', COLOR_S , 1, flags);
		else if (sh && ((sh == 6 && perms & S_IXUSR && perms & S_ISUID)
				|| (perms & S_IXGRP && perms & S_ISGID)))
			ft_putstr_color_fd('s', COLOR_S, 1, flags);
		else
		{
			if (!sh && perms & S_ISVTX)
				ft_putstr_color_fd(perms & 1 << sh ? 't' : 'T', COLOR_T, 1, flags);
			else
				ft_putstr_color_fd(perms & 1 << sh ? 'x' : '-', COLOR_EXEC, 1, flags);
		}
	}
	ft_putstr_fd("  ", 1);
}

void	pad(ssize_t c)
{
	while (c--)
		ft_putchar_fd(' ', 1);
}

void	print_date(const time_t *timestamp)
{
	const	time_t	now = time(NULL);
	const 	time_t	diff = *timestamp > now ? *timestamp - now : now - *timestamp;
	char	*date = ctime(timestamp);

	if (diff >= 6 * MONTH)
	{
		write(1, date + 4, 7);
		write(1, date + 19, 5);
	}
	else
		write(1, date + 4, 12);
	write(1, " ", 1);
}

void	print_color_file(t_payload *payload, uint8_t flags)
{
		ft_putf_fd(1, "%s%s%s\n", color_code(payload, flags), payload->d_shname, (flags & FLAG_COLORS_ON) ? COLOR_RESET : "");
}

void	long_format(t_payload *payload, uint8_t flags, t_maxs *maximums)
{
	const uint8_t	special_device = S_ISCHR(payload->stats.st_mode)
		|| S_ISBLK(payload->stats.st_mode);

	print_file_mode(payload->stats.st_mode, flags);
	pad(maximums->links_len - nb_len(payload->stats.st_nlink));
	ft_putf_fd(1, "%d ", payload->stats.st_nlink);
	ft_putstr_fd(payload->user, 1);
	pad(maximums->user - ft_strlen(payload->user) + 2);
	ft_putf_fd(1, "%s  ", payload->group);
	pad(maximums->group - ft_strlen(payload->group));
	pad(special_device ?
		maximums->major_len - nb_len(payload->stats.st_rdev >> 24)
		: maximums->size_len - nb_len(payload->stats.st_size));
	if (special_device)
	{
		ft_putf_fd(1, "%d, ", payload->stats.st_rdev >> 24);
		pad(maximums->minor_len - nb_len(payload->stats.st_rdev & 0xFF));
		ft_putf_fd(1, "%d ", payload->stats.st_rdev & 0xFF);
	}
	else
	{
		if (maximums->major || maximums->minor)
			pad(maximums->major_len + maximums->minor_len + 1);
		ft_putf_fd(1, "%d ", payload->stats.st_size);
	}
	print_date(&payload->stats.st_mtimespec.tv_sec);
	print_color_file(payload, flags);
}

void	list_file(t_payload *payload, uint8_t flags, t_maxs *maximums)
{
	if (!(flags & FLAG_LONG_FORMAT))
		print_color_file(payload, flags);
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

	if (len > *size)
		*size = len;
}

void	set_major_minor(t_maxs *maximums, dev_t st_rdev)
{
	const unsigned int	major = st_rdev >> 24;
	const unsigned int	minor = st_rdev & 0xFF;

	if (maximums->major < major)
		maximums->major = major;
	if (maximums->minor < minor)
		maximums->minor = minor;
}

void	list_dir(t_payload *stats, uint8_t flags, uint8_t print_name)
{
	DIR				*directory;
	t_entries		entries;
	struct dirent	*d;
	int				i;
	char			*short_name;
	t_maxs			maximums;

	maximums = (t_maxs) { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
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
			set_longer_string(&(maximums.user), entries.payloads[i]->user);
			set_longer_string(&(maximums.group), entries.payloads[i]->group);
			maximums.blocks += entries.payloads[i]->stats.st_blocks;
			set_major_minor(&maximums, entries.payloads[i++]->stats.st_rdev);
		}
	}
	maximums.links_len = nb_len(maximums.links);
	maximums.size_len = nb_len(maximums.size);
	maximums.major_len = nb_len(maximums.major);
	maximums.minor_len = nb_len(maximums.minor);
	if (entries.len > 1)
		quick_sort((void **)entries.payloads , 0, entries.len - 1, ft_d_name_sort, flags);
	if (flags & FLAG_LONG_FORMAT)
		ft_putf_fd(1, "total %d\n", maximums.blocks);
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
