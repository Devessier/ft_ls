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
#include "collect.h"
#include <stdbool.h>

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

int		error(char *path, t_uflag flags)
{
	ft_putf_fd(2, "ft_ls: %s: %s%s%s\n", path, flags & FLAG_COLORS_ON
		? COLOR_EXEC : "", strerror(errno),
		flags & FLAG_COLORS_ON ? COLOR_RESET : "");
	errno = 0;
	return (1);
}

void	usage(char c)
{
	uint8_t	i;

	i = 0;
	ft_putf_fd(2, "ft_ls: illegal option -- %c\n", c);
	ft_putstr_fd("usage: ft_ls [-", 2);
	while (g_arguments[i].c_flag)
		ft_putchar_fd(g_arguments[i++].c_flag, 2);
	ft_putstr_fd("] [file ...]\n", 2);
	exit(1);
}

char	*color_code(mode_t st_mode, t_uflag flags)
{
	uint8_t			i;

	if (!(flags & FLAG_COLORS_ON))
		return ("");
	if (S_ISDIR(st_mode) && (st_mode & S_IWUSR
			|| st_mode & S_IWGRP) && st_mode & S_IWOTH)
		return (st_mode & S_ISVTX ? COLOR_SUPER_DIR_SAVE : COLOR_SUPER_DIR);
	if (S_ISREG(st_mode) && st_mode & S_ISUID && st_mode & S_AEXEC)
		return (COLOR_UID);
	if (S_ISREG(st_mode) && st_mode & S_ISGID && st_mode & S_AEXEC)
		return (COLOR_GID);
	i = 0;
	while (g_file_types[i].mode)
		if ((st_mode & S_IFMT) == g_file_types[i++].mode
				&& *g_file_types[i - 1].color)
			return (g_file_types[i - 1].color);
	return (st_mode & S_IXUSR ? COLOR_EXEC : "");
}

void	print_file_type(mode_t perms, t_uflag flags)
{
	uint8_t	i;

	i = 0;
	while (g_file_types[i].mode)
		if ((perms & S_IFMT) == g_file_types[i++].mode)
			ft_putchar_color_fd(g_file_types[i - 1].to_char,
					g_file_types[i - 1].color, 1, flags);
}

void	print_sticky_bit(int8_t sh, mode_t perms, t_uflag flags)
{
	if (!sh && perms & S_ISVTX)
		ft_putchar_color_fd(perms & 1 << sh ? 't' : 'T',
			COLOR_T, 1, flags);
	else
		ft_putchar_color_fd(perms & 1 << sh ? 'x' : '-',
			COLOR_EXEC, 1, flags);
}

void	print_file_mode(mode_t perms, t_uflag flags)
{
	int8_t	sh;

	print_file_type(perms, flags);
	sh = 9;
	while ((sh -= 3) >= 0)
	{
		ft_putchar_color_fd((perms & 4 << sh) ? 'r' : '-',
			COLOR_READ, 1, flags);
		ft_putchar_color_fd((perms & 2 << sh) ? 'w' : '-',
			COLOR_WRITE, 1, flags);
		if (sh && ((sh == 6 && !(perms & S_IXUSR) && perms & S_ISUID)
				|| (sh == 3 && !(perms & S_IXGRP) && perms & S_ISGID)))
			ft_putchar_color_fd('S', COLOR_S, 1, flags);
		else if (sh && ((sh == 6 && perms & S_IXUSR && perms & S_ISUID)
				|| (perms & S_IXGRP && perms & S_ISGID)))
			ft_putchar_color_fd('s', COLOR_S, 1, flags);
		else
			print_sticky_bit(sh, perms, flags);
	}
	ft_putstr_fd("  ", 1);
}

void	pad(int64_t c)
{
	while (c--)
		ft_putchar_fd(' ', 1);
}

void	print_date(t_payload *payload, t_uflag flags)
{
	const	time_t	now = time(NULL);
	time_t			diff;
	time_t			timestamp;
	char			*date;

	if (flags & FLAG_LAST_ACCESS)
		timestamp = payload->stats.st_atimespec.tv_sec;
	else if (flags & FLAG_CREATION)
		timestamp = payload->stats.st_birthtimespec.tv_sec;
	else if (flags & FLAG_STATUS_CHANGED)
		timestamp = payload->stats.st_ctimespec.tv_sec;
	else
		timestamp = payload->stats.st_mtimespec.tv_sec;
	diff = timestamp > now ? timestamp - now : now - timestamp;
	date = ctime(&timestamp);
	if (diff > 3600 * 24 * 30.5 * 6)
	{
		write(1, date + 4, 7);
		write(1, date + 19, 5);
	}
	else
		write(1, date + 4, 12);
	write(1, " ", 1);
}

void	print_color_file(t_payload *payload, t_uflag flags)
{
	ft_putf_color_fd(1, color_code(payload->stats.st_mode, flags),
		flags, "%s", payload->d_shname);
	if (flags & FLAG_LONG_FORMAT && S_ISLNK(payload->stats.st_mode))
		ft_putf_fd(1, " -> %s", payload->link);
	ft_putchar_fd('\n', 1);
}

void	long_format(t_payload *p, t_uflag flags, t_maxs *maximums)
{
	const t_uflag	sd = S_ISCHR(p->stats.st_mode) || S_ISBLK(p->stats.st_mode);

	print_file_mode(p->stats.st_mode, flags);
	pad(maximums->links_len - nb_len(p->stats.st_nlink));
	ft_putf_fd(1, "%d %s", p->stats.st_nlink, p->user);
	pad(maximums->user - ft_strlen(p->user) + 2);
	ft_putf_fd(1, "%s  ", p->group);
	pad(maximums->group - ft_strlen(p->group));
	pad(sd ?
		maximums->major_len - nb_len(p->stats.st_rdev >> 24)
		: maximums->size_len - nb_len(p->stats.st_size));
	if (sd)
	{
		ft_putf_fd(1, "%d, ", p->stats.st_rdev >> 24);
		pad(maximums->minor_len - nb_len(p->stats.st_rdev & 0xFF));
		ft_putf_fd(1, "%d ", p->stats.st_rdev & 0xFF);
	}
	else
	{
		if (maximums->major || maximums->minor)
			pad(maximums->major_len + maximums->minor_len + 1);
		ft_putf_fd(1, "%d ", p->stats.st_size);
	}
	print_date(p, flags);
	print_color_file(p, flags);
}

void	list_file(t_payload *payload, t_uflag flags, t_maxs *maximums)
{
	if (!(flags & FLAG_LONG_FORMAT))
		print_color_file(payload, flags);
	else
		long_format(payload, flags, maximums);
}

void	free_stats(t_payload *stats, t_uflag flags, bool free_names, bool link)
{
	if (free_names)
	{
		free(stats->d_shname);
		free(stats->d_name);
	}
	if (flags & FLAG_LONG_FORMAT)
	{
		free(stats->user);
		free(stats->group);
	}
	if (link && flags & FLAG_LONG_FORMAT)
		free(stats->link);
	free(stats);
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

void	free_entries(t_entries *entries, t_uflag flags)
{
	int	i;

	i = 0;
	while (i++ < entries->len)
		free_stats(entries->payloads[i - 1], flags, 1,
			S_ISLNK(entries->payloads[i - 1]->stats.st_mode));
	if (entries->payloads)
		free(entries->payloads);
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
