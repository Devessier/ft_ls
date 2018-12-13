/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 20:03:51 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/13 14:18:58 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"
#include "list.h"
#include <stdio.h>
#include <sys/stat.h>
#include "utils.h"
#include <unistd.h>
#include "utils.h"
#include "sort.h"
#include "collect.h"

t_uflag	parse_flags(char *flag)
{
	t_uflag	flags;
	t_uflag	i;
	uint8_t found;

	flags = FLAG_NONE;
	while (*++flag)
	{
		found = 0;
		i = 0;
		while (g_arguments[i].c_flag)
			if (g_arguments[i++].c_flag == *flag)
			{
				flags |= g_arguments[i - 1].flag;
				found = 1;
			}
		if (!found)
			usage(*flag);
	}
	if ((flags & FLAG_COLORS_ON) && !isatty(1))
		flags ^= FLAG_COLORS_ON;
	return (flags);
}

t_uflag		flags(char **args, int len, int *i)
{
	t_uflag	flags;

	flags = isatty(1) << 5;
	*i = 0;
	while (*i < len && *args[*i] == '-' && args[*i][1])
	{
		if (args[*i][1] == '-')
		{
			++*i;
			break ;
		}
		flags |= parse_flags(args[*i]);
		++*i;
	}
	return (flags);
}

int			main(int len, char **args)
{
	int				i;
	const t_uflag	fl = flags(++args, --len, &i);
	const int		total_args = len - i;
	t_uflag			args_sort_fl;

	args_sort_fl = fl;
	if (fl & FLAG_REVERSE_SORT)
		args_sort_fl ^= FLAG_REVERSE_SORT;
	quick_sort((void **)args, i, len - 1, ft_strcmp, args_sort_fl);
	collect_entries(args + i, total_args, fl);
}
