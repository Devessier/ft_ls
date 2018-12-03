/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 10:57:31 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/03 17:46:17 by bdevessi         ###   ########.fr       */
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

char	*color_code(t_payload *payload, uint8_t flags)
{
	const mode_t	st_mode = payload->stats.st_mode;
	if (!(flags & FLAG_COLORS_ON))
		return ("");
	if (S_ISFIFO(st_mode))
		return (COLOR_FIFO);
	if (S_ISCHR(st_mode))
		return (COLOR_CHR);
	if (S_ISBLK(st_mode))
		return (COLOR_BLK);
	if (S_ISDIR(st_mode))
		return (COLOR_DIR);
	if (S_ISLNK(st_mode))
		return (COLOR_LNK);
	if (S_ISSOCK(st_mode))
		return (COLOR_SOCK);
	return ("");
}

void	list_file(t_payload *stats, uint8_t flags)
{
	if (!(flags & FLAG_LONG_FORMAT))
		ft_putf_fd(1, "%s%s%s\n", color_code(stats, flags), stats->d_shname, (flags & FLAG_COLORS_ON) ? COLOR_RESET : "");
}

void	free_stats(t_payload *stats)
{
	free(stats->d_shname);
	free(stats->d_name);
	free(stats);
}

void	list_dir(t_payload *stats, uint8_t flags, uint8_t print_name)
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
		if (append_entry(&entries, &entries, pathjoin(stats->d_name, d->d_name), strdup(d->d_name), 1))
			return ;
	}
	if (entries.len > 1)
		quick_sort((void **)entries.payloads , 0, entries.len - 1, ft_d_name_sort, flags);
	i = 0;
	while (i < entries.len)
		list_file(entries.payloads[i++], entries.flags);
	i = 0;
	while ((flags & FLAG_RECURSIVE) && i < entries.len)
	{
		if (S_ISDIR(entries.payloads[i++]->stats.st_mode))
		{
			list_dir(entries.payloads[i - 1], flags, 1);
			free_stats(entries.payloads[i - 1]);
		}
		else
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
		list_file(argstat, flags);
}
