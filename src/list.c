/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 10:57:31 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/03 12:42:44 by bdevessi         ###   ########.fr       */
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

int		error(char *path)
{
	ft_putf_fd(2, "ft_ls: %s: %s\n", path, strerror(errno))	;
	return (1);
}

char	*color_code(t_stat *stats)
{
	if (S_ISFIFO(stats->st_mode))
		return (COLOR_FIFO);
	if (S_ISCHR(stats->st_mode))
		return (COLOR_CHR);
	if (S_ISBLK(stats->st_mode))
		return (COLOR_BLK);
	if (S_ISDIR(stats->st_mode))
		return (COLOR_DIR);
	if (S_ISLNK(stats->st_mode))
		return (COLOR_LNK);
	if (S_ISSOCK(stats->st_mode))
		return (COLOR_SOCK);
	return (COLOR_RESET);
}

void	list_file(t_stat *stats, uint8_t flags)
{
	if (!(flags & FLAG_LONG_FORMAT))
	{
		ft_putf_fd(1, "%s%s" COLOR_RESET "\n", color_code(stats), stats->d_shname);
		ft_putstr_fd(COLOR_RESET, 1);
	}
}

void	free_stats(t_stat *stats)
{
	free(stats->d_shname);
	free(stats->d_name);
	free(stats);
}

void	list_dir(t_stat *stats, uint8_t flags, uint8_t print_name)
{
	DIR		*directory;
	t_entries	entries;
	struct dirent	*d;
	int		i;

	if (print_name)
		ft_putf_fd(1, "\n%s:\n", stats->d_name);
	entries = (t_entries){ flags, 0, 0, NULL };
	errno = 0;
	if (!(directory = opendir(stats->d_name)))
		return ((void)error(stats->d_name));
	while ((d = readdir(directory)) != NULL)
	{
		if (*d->d_name == '.' && !(flags & FLAG_INCLUDE_DOTS))
			continue ;
		if (append_entry(&entries, pathjoin(stats->d_name, d->d_name), strdup(d->d_name), 1))
			return ;
	}
	if (entries.len > 1)
		quick_sort((void **)entries.stats, 0, entries.len - 1, ft_d_name_sort);
	i = 0;
	while (i < entries.len)
		list_file(entries.stats[i++], entries.flags);
	i = 0;
	while ((flags & FLAG_RECURSIVE) && i < entries.len)
	{
		if (S_ISDIR(entries.stats[i++]->st_mode))
		{
			list_dir(entries.stats[i - 1], flags, 1);
			free_stats(entries.stats[i - 1]);
		}
		else
			free_stats(entries.stats[i - 1]);
	}
	closedir(directory);
	free(entries.stats);
}

void	list_argument(t_stat *argstat, uint8_t flags)
{
	if (!argstat->st_mode)
		return ;
	if (S_ISDIR(argstat->st_mode))
		list_dir(argstat, flags, 0);
	else
		list_file(argstat, flags);
}
