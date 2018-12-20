/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 10:38:07 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/20 10:38:08 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "ft_ls.h"

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

void	print_file_mode(t_payload *payload, t_uflag flags)
{
	const mode_t	perms = payload->stats.st_mode;
	int8_t			sh;

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
	if (payload->has_ea || payload->has_acl)
		ft_putf_color_fd(1, payload->has_ea ? "\033[38;2;95;175;175m"
		: "\033[38;2;95;255;0m", flags, payload->has_ea ? "@ " : "+ ");
	else
		ft_putstr_fd("  ", 1);
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
	if (diff > 3600 * 24 * 30 * 6)
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
