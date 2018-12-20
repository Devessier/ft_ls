/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 20:03:51 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/18 13:24:39 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include "ft_ls.h"

void	xor_flags(char c, t_uflag *flags)
{
	if (c == 'c')
	{
		*flags &= ~(FLAG_CREATION);
		*flags &= ~(FLAG_LAST_ACCESS);
	}
	if (c == 'U')
	{
		*flags &= ~(FLAG_STATUS_CHANGED);
		*flags &= ~(FLAG_LAST_ACCESS);
	}
	if (c == 'u')
	{
		*flags &= ~(FLAG_STATUS_CHANGED);
		*flags &= ~(FLAG_CREATION);
	}
	if (c == '1')
		*flags &= ~(FLAG_LONG_FORMAT);
	if (c == 'l')
		*flags &= ~(FLAG_ONE_ENTRY_PER_LINE);
}

void	parse_flags(char *flag, t_uflag *flags)
{
	uint8_t	i;
	uint8_t found;

	while (*++flag)
	{
		found = 0;
		i = 0;
		while (g_arguments[i].c_flag)
			if (g_arguments[i++].c_flag == *flag)
			{
				*flags |= g_arguments[i - 1].flag;
				xor_flags(g_arguments[i - 1].c_flag, flags);
				found = 1;
			}
		if (!found)
			usage(*flag);
	}
}

t_uflag	flags(char **args, int len, int *i)
{
	t_uflag		flags;
	const int	is_tty = isatty(1);

	flags = is_tty << 5;
	*i = 0;
	while (*i < len && *args[*i] == '-' && args[*i][1])
	{
		if (args[*i][1] == '-')
		{
			++*i;
			break ;
		}
		parse_flags(args[*i], &flags);
		if (flags & FLAG_COLORS_ON && !is_tty)
			flags ^= FLAG_COLORS_ON;
		++*i;
	}
	return (flags);
}

int		main(int len, char **args)
{
	int				i;
	const t_uflag	fl = flags(++args, --len, &i);
	const int		total_args = len - i;
	t_uflag			args_sort_fl;

	args_sort_fl = fl;
	if (fl & FLAG_REVERSE_SORT)
		args_sort_fl ^= FLAG_REVERSE_SORT;
	quick_sort((void**)(args + i), (t_sort_args) {
		args_sort_fl,
		0,
		total_args - 1
	}, ft_strcmp);
	collect_entries(args + i, total_args, fl);
}
