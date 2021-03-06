/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 13:34:54 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/18 20:32:30 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include "ft_ls.h"

void	set_dot(t_entries *dir, t_uflag flags)
{
	struct stat	s;

	errno = 0;
	if (stat(".", &s))
		error(".", flags);
	else
		append_entry(dir, s, ".", ".");
}

void	collect_entries(char **args, int len, t_uflag flags)
{
	const t_entries	files = (t_entries) { flags, 0, 0, 0 };
	const t_entries	dir = (t_entries) { flags, 0, 0, 0 };
	int				i[2];
	struct stat		stats;
	t_maxs			files_maxs;

	i[0] = -1;
	i[1] = 0;
	files_maxs = (t_maxs) { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	if (!len)
		set_dot((t_entries*)&dir, flags);
	while (++*i < len)
	{
		errno = 0;
		if ((flags & FLAG_LONG_FORMAT ? lstat : stat)(args[*i], &stats) != 0)
			error(args[*i], flags);
		else if (append_entry((t_entries*)(S_ISDIR(stats.st_mode) ? &dir :
				&files), stats, args[*i], args[*i]) == 0)
			if (flags & FLAG_LONG_FORMAT && !S_ISDIR(stats.st_mode))
				update_maximums(files.payloads[i[1]++], &files_maxs);
	}
	calculate_max_len(&files_maxs);
	sort_entries((void**)files.payloads, 0, files.len - 1, flags);
	sort_entries((void**)dir.payloads, 0, dir.len - 1, flags);
	print(&files, &dir, len, &files_maxs);
}

void	update_maximums(t_payload *payload, t_maxs *maximums)
{
	const dev_t		major = payload->stats.st_rdev >> 24;
	const dev_t		minor = payload->stats.st_rdev & 0xFF;
	const size_t	usr_len = ft_strlen(payload->user);
	const size_t	grp_len = ft_strlen(payload->group);

	if (maximums->major < major)
		maximums->major = major;
	if (maximums->minor < minor)
		maximums->minor = minor;
	if (payload->stats.st_nlink > maximums->links)
		maximums->links = payload->stats.st_nlink;
	if (payload->stats.st_size > maximums->size)
		maximums->size = payload->stats.st_size;
	if (usr_len > maximums->user)
		maximums->user = usr_len;
	if (grp_len > maximums->group)
		maximums->group = grp_len;
	maximums->blocks += payload->stats.st_blocks;
}

void	rd_dir_hdl_err(struct stat *s, char *path, t_uflag flags)
{
	if (s->st_mode)
		error(path, flags);
	free(path);
}

uint8_t	read_directory(const t_entries *entries,
	t_payload *stats, t_uflag flags, t_maxs *maximums)
{
	DIR				*directory;
	struct dirent	*d;
	struct stat		s;
	char			*path;
	int				i;

	errno = 0;
	if (!(directory = opendir(stats->d_name)))
		return (error(stats->d_shname, flags));
	i = 0;
	while ((d = readdir(directory)) != NULL)
	{
		if (*d->d_name == '.' && !(flags & FLAG_INCLUDE_DOTS))
			continue ;
		path = pathjoin(stats->d_name, d->d_name);
		errno = 0;
		if (lstat(path, &s) ||
			append_entry(((t_entries *)entries), s, path, ft_strdup(d->d_name)))
			rd_dir_hdl_err(&s, path, flags);
		else if (flags & FLAG_LONG_FORMAT)
			update_maximums(entries->payloads[i++], maximums);
	}
	return (closedir(directory) & 0);
}
