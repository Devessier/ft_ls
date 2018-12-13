/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 13:04:38 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/13 16:36:05 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <sys/stat.h>
#include "args.h"
#include "sort.h"
#include <stdio.h>

int		ft_strcmp(void *s1, void *s2, t_uflag flags)
{
	(void)flags;
	size_t		i;
	char		*c_1;
	char		*c_2;

	i = 0;
	c_1 = (char *)s1;
	c_2 = (char *)s2;
	while (c_1[i] == c_2[i] && c_1[i] && c_2[i])
		i++;
	return ((unsigned char)c_1[i] - (unsigned char)c_2[i]);
}

int		time_sort(void *d1, void *d2, t_uflag flags)
{
	const struct stat	s1 = ((t_payload *)d1)->stats;
	const struct stat	s2 = ((t_payload *)d2)->stats;
	struct timespec		time1;
	struct timespec		time2;
	time_t				diff;

	if (flags & FLAG_LAST_ACCESS)
	{
		time1 = s1.st_atimespec;
		time2 = s2.st_atimespec;
	}
	else if (flags & FLAG_CREATION)
	{
		time1 = s1.st_birthtimespec;
		time2 = s2.st_birthtimespec;
	}
	else if (flags & FLAG_STATUS_CHANGED)
	{
		time1 = s1.st_ctimespec;
		time2 = s2.st_ctimespec;
	}
	else
	{
		time1 = s1.st_mtimespec;
		time2 = s2.st_mtimespec;
	}
	diff = time2.tv_sec - time1.tv_sec;
	if (diff == 0)
		return (ft_d_name_sort(d1, d2, flags));
	return (diff > 0 ? 1 : -1);
}

int		ft_d_name_sort(void *d1, void *d2, t_uflag flags)
{
	return (ft_strcmp((void *)(((t_payload *)d1)->d_name),
				(void *)(((t_payload *)d2)->d_name), flags));
}

void	swap(void **a, void **b)
{
	void	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	quick_sort(void **list, int start, int end, int (*f)(void*, void*, t_uflag), t_uflag flags)
{
	void			*key;
	int				mid;
	int				i;
	int				j;
	const t_uflag	reverse_order = flags & FLAG_REVERSE_SORT;

	if (!(start < end))
		return ;
	mid = (start + end) / 2;
	swap(list + start, list + mid);
	key = list[start];
	i = start + 1;
	j = end;
	while (i <= j)
	{
		while (i <= end && (reverse_order ? f(list[i], key, flags) >= 0 : f(list[i], key, flags) <= 0))
			i++;
		while (j >= start && (!reverse_order ? f(list[j], key, flags) > 0 : f(list[j], key, flags) < 0))
			j--;
		if (i < j)
			swap(list + i, list + j);
	}
	swap(list + start, list + j);
	quick_sort(list, start, j - 1, f, flags);
	quick_sort(list, j + 1, end, f, flags);
}

void	sort_entries(void **list, int start, int end, t_uflag flags)
{
	if (flags & FLAG_SORT_TIME_MODIFIED)
		quick_sort(list, start, end, time_sort, flags);
	else
		quick_sort(list, start, end, ft_d_name_sort, flags);
}
