/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 20:03:51 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/04 16:26:30 by bdevessi         ###   ########.fr       */
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

int		normalize_argument(char **str)
{
	int		i;

	i = 0;
	while ((*str)[i])
		i++;
	if (i)
		i--;
	while (i && (*str)[i] == '/')
		(*str)[i--] = '\0';
	return (i);
}

uint8_t		parse_flags(char *flag)
{
	uint8_t	flags;
	uint8_t	i;

	flags = FLAG_NONE;
	while (*++flag)
	{
		i = 0;
		while (g_arguments[i].c_flag)
			if (g_arguments[i++].c_flag == *flag)
				flags |= g_arguments[i - 1].flag;
	}
	if ((flags & FLAG_COLORS_ON) && !isatty(1))
		flags ^= FLAG_COLORS_ON;
	return (flags);
}

void		print_entry(t_entries *entry, int total_args)
{
	int	i;

	i = 0;
	while (i < entry->len)
	{
		list_argument(entry->payloads[i], entry->flags, total_args > entry->len);
		if (entry->len - i++ > 1 && S_ISDIR(entry->payloads[i]->stats.st_mode))
			ft_putchar_fd('\n', 1);
	}
}

int	main(int len, char **args)
{
	t_entries	files_args;
	t_entries	dir_args;
	int			i;
	uint8_t		flags;
	int			total_args;

	--len;
	++args;
	flags = isatty(1) ? FLAG_COLORS_ON : FLAG_NONE;
	i = 0;
	while (i < len && *args[i] == '-')
	{
		if (args[i][1] == '-')
			break ;
		else
			flags |= parse_flags(args[i]);
		i++;
	}
	files_args.flags = flags;
	dir_args.flags = flags;
	if (len == i)
		append_entry(&dir_args, &dir_args, ".", ".", 0);
	total_args = len - i;
	while (i < len)
	{
		normalize_argument(&args[i]);
		append_entry(&dir_args, &files_args, args[i], args[i], 0);
		i++;
	}
	if (files_args.len > 1)
		quick_sort((void **)files_args.payloads, 0, files_args.len - 1, args_sort, files_args.flags);
	if (dir_args.len > 1)
		quick_sort((void **)dir_args.payloads, 0, dir_args.len - 1, args_sort, dir_args.flags);
	print_entry(&files_args, total_args);
	print_entry(&dir_args, total_args);
}
