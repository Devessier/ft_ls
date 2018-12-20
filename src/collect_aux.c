/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 14:46:12 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/20 14:46:13 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <stdlib.h>

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

void	print_entry(const t_entries *entry, int total_args)
{
	int	i;

	i = 0;
	while (i < entry->len)
	{
		if (S_ISDIR(entry->payloads[i]->stats.st_mode)
			&& total_args > 1 && total_args >= entry->len)
		{
			if (i && i < total_args)
				ft_putchar_fd('\n', 1);
			ft_putf_fd(1, "%s:\n", entry->payloads[i]->d_name);
		}
		list_argument(entry->payloads[i++], entry->flags);
		free_stats(entry->payloads[i - 1], entry->flags, 0,
			S_ISLNK(entry->payloads[i - 1]->stats.st_mode));
	}
	if (entry->payloads)
		free(entry->payloads);
}

void	print(const t_entries *files_args,
	const t_entries *dir_args, int len, t_maxs *f_maxs)
{
	int i;

	i = 0;
	while (i < files_args->len)
	{
		if (files_args->payloads[i++]->stats.st_mode)
			list_file(files_args->payloads[i - 1], files_args->flags, f_maxs);
		free_stats(files_args->payloads[i - 1], files_args->flags, 0,
			S_ISLNK(files_args->payloads[i - 1]->stats.st_mode));
	}
	if (files_args->payloads)
		free(files_args->payloads);
	if (files_args->len && files_args->len
			< files_args->len + dir_args->len)
		ft_putchar_fd('\n', 1);
	print_entry(dir_args, len);
}

void	sort_entries(void **list, int start, int end, t_uflag flags)
{
	quick_sort(list, (t_sort_args) { flags, start, end },
		flags & FLAG_SORT_TIME_MODIFIED ?
			time_sort :
			ft_d_name_sort);
}
