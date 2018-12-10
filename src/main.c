/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 20:03:51 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/10 16:59:09 by bdevessi         ###   ########.fr       */
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

t_uflag	parse_flags(char *flag)
{
	t_uflag	flags;
	t_uflag	i;
	t_uflag found;

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

void		print_entry(t_entries *entry, int total_args)
{
	int	i;

	i = 0;
	while (i < entry->len)
	{
		if (S_ISDIR(entry->payloads[i]->stats.st_mode) && total_args > 1 && total_args >= entry->len)
			ft_putf_fd(1, "%c%s:\n", (i && i < total_args) ? '\n' : 0 , entry->payloads[i]->d_name);
		list_argument(entry->payloads[i++], entry->flags);
	}
}

int	main(int len, char **args)
{
	t_entries	files_args;
	t_entries	dir_args;
	int			i;
	t_uflag		flags;
	int			total_args;

	--len;
	++args;
	flags = isatty(1) ? FLAG_COLORS_ON : FLAG_NONE;
	i = 0;
	files_args = (t_entries) { 0, 0, 0, NULL };
	dir_args = (t_entries) { 0, 0, 0, NULL };
	while (i < len && *args[i] == '-' && args[i][1])
	{
		i++;
		if (args[i - 1][1] == '-')
			break ;
		else
			flags |= parse_flags(args[i - 1]);
	}
	files_args.flags = flags;
	dir_args.flags = flags;
	if (len == i)
		append_entry(&dir_args, &dir_args, ".", ".", 0);
	total_args = len - i;
	quick_sort((void **)args, i, len - 1, ft_strcmp, flags);
	while (++i <= len)
		append_entry(&dir_args, &files_args, strdup(args[i - 1]), normalize_argument(&args[i - 1]), 0);
	quick_sort((void **)files_args.payloads, 0, files_args.len - 1, args_sort, files_args.flags);
	quick_sort((void **)dir_args.payloads, 0, dir_args.len - 1, args_sort, dir_args.flags);
	print_entry(&files_args, total_args);
	if (files_args.len && files_args.len < total_args)
		ft_putchar_fd('\n', 1);
	print_entry(&dir_args, total_args);
}
