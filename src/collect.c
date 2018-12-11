/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 13:34:54 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/11 13:51:15 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include "utils.h"
#include "sort.h"
#include "list.h"
#include <stdio.h>

char	*normalize_argument(char **str)
{
	int			i;
	const char	*start = *str;

	i = 0;
	while ((*str)[i])
		i++;
	if (i)
		i--;
	while (i && (*str)[i] == '/')
		(*str)[i--] = '\0';
	return ((char *)start);
}

void		print_entry(const t_entries *entry, int total_args)
{
	int	i;

	i = 0;
	while (i < entry->len)
	{
		if (S_ISDIR(entry->payloads[i]->stats.st_mode)
			&& total_args > 1 && total_args >= entry->len)
			ft_putf_fd(1, "%c%s:\n", i && i < total_args ? '\n' : 0,
				entry->payloads[i]->d_name);
		list_argument(entry->payloads[i++], entry->flags);
	}
}

void	print(const t_entries *files_args, const t_entries *dir_args, int len)
{
	print_entry(files_args, len);
	if (files_args->len && files_args->len < len)
		ft_putchar_fd('\n', 1);
	print_entry(dir_args, len);
}

int		collect_entries(char **args, int len, t_uflag flags)
{
	const t_entries	files = (t_entries) { flags, 0, 0, 0 };
	const t_entries	dir = (t_entries) { flags, 0, 0, 0 };
	int				i;
	struct stat		s;

	i = 0;
	if (!len)
	{
		errno = 0;
		if (stat(".", &s))
			error(*args);
		append_entry((t_entries *)&dir, s, ".", ".");
	}
	while (i < len)
	{
		errno = 0;
		if ((flags & FLAG_LONG_FORMAT ? lstat : stat)(args[i], &s) != 0)
			error(args[i]);
		append_entry((t_entries *)(S_ISDIR(s.st_mode) ? &dir : &files),
			s, args[i], normalize_argument(&args[i]));
		i++;
	}
	quick_sort((void**)files.payloads, 0, files.len - 1, args_sort, flags);
	quick_sort((void**)dir.payloads, 0, dir.len - 1, args_sort, flags);
	print(&files, &dir, len);
	return (0);
}

void	set_longer_string(unsigned int *size, char *str)
{
	const size_t	len = ft_strlen(str);

	if (len > *size)
		*size = len;
}

void	update_maximums(t_payload *payload, t_maxs *maximums)
{
	const unsigned int	major = payload->stats.st_rdev >> 24;
	const unsigned int	minor = payload->stats.st_rdev & 0xFF;

	if (maximums->major < major)
		maximums->major = major;
	if (maximums->minor < minor)
		maximums->minor = minor;
	if (payload->stats.st_nlink > maximums->links)
		maximums->links = payload->stats.st_nlink;
	if (payload->stats.st_size > maximums->size)
		maximums->size = payload->stats.st_size;
	set_longer_string(&(maximums->user), payload->user);
	set_longer_string(&(maximums->group), payload->group);
	maximums->blocks += payload->stats.st_blocks;
}

void	read_directory(const t_entries *entries,
	t_payload *stats, t_uflag flags, t_maxs *maximums)
{
	DIR				*directory;
	struct dirent	*d;
	struct stat		s;
	char			*path;
	int				i;

	errno = 0;
	if (!(directory = opendir(stats->d_name)))
		return ((void)error(stats->d_name));
	i = 0;
	while ((d = readdir(directory)) != NULL)
	{
		if (*d->d_name == '.' && !(flags & FLAG_INCLUDE_DOTS))
			continue ;
		path = pathjoin(stats->d_name, d->d_name);
		errno = 0;
		if ((flags & FLAG_LONG_FORMAT ? lstat : stat)(path, &s))
			error(path);
		if (append_entry(((t_entries *)entries), s, path, ft_strdup(d->d_name)))
			return ;
		if (flags & FLAG_LONG_FORMAT)
			update_maximums(entries->payloads[i], maximums);
		i++;
	}
	closedir(directory);
}
