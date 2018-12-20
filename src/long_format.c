/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   long_format.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 10:57:18 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/20 10:57:18 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	long_format(t_payload *p, t_uflag flags, t_maxs *maximums)
{
	const t_uflag	sd = S_ISCHR(p->stats.st_mode) || S_ISBLK(p->stats.st_mode);

	print_file_mode(p, flags);
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

time_t	set_time(const struct stat *stat, t_uflag flags)
{
	if (flags & FLAG_LAST_ACCESS)
		return (stat->st_atime);
	if (flags & FLAG_CREATION)
		return (stat->st_birthtime);
	if (flags & FLAG_STATUS_CHANGED)
		return (stat->st_ctime);
	return (stat->st_mtime);
}

int		time_diff(void *d1, void *d2, t_uflag flags)
{
	const time_t	time1 = set_time(&((t_payload *)d1)->stats, flags);
	const time_t	time2 = set_time(&((t_payload *)d2)->stats, flags);

	return (time2 - time1);
}
