/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 10:57:31 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/02 01:34:25 by bdevessi         ###   ########.fr       */
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

void	error(char *path)
{
	ft_putf_fd(2, "ft_ls: %s: %s\n", path, strerror(errno))	;
}

void	list_file(t_stat *stats, uint8_t flags)
{
	if (!(flags & FLAG_LONG_FORMAT))
		ft_putf_fd(1, "%s\n", stats->d_shname);
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
		error(stats->d_name);
	while ((d = readdir(directory)) != NULL)
	{
		if (*d->d_name == '.' && !(flags & FLAG_INCLUDE_DOTS))
			continue ;
		append_entry(&entries, pathjoin(stats->d_name, d->d_name), strdup(d->d_name));
	}
	if (entries.len > 1)
		quick_sort((void **)entries.stats, 0, entries.len -1, ft_d_name_sort);
	i = 0;
	while (i < entries.len)
		list_file(entries.stats[i++], entries.flags);
	i = 0;
	while ((flags & FLAG_RECURSIVE) && i < entries.len)
		if (S_ISDIR(entries.stats[i++]->st_mode))
			list_dir(entries.stats[i - 1], flags, 1);
	closedir(directory);
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
