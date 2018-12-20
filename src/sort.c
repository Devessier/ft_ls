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
#include "ft_ls.h"

int		ft_strcmp(void *s1, void *s2, t_uflag flags)
{
	size_t		i;
	char		*c_1;
	char		*c_2;

	(void)flags;
	i = 0;
	c_1 = (char *)s1;
	c_2 = (char *)s2;
	while (c_1[i] == c_2[i] && c_1[i] && c_2[i])
		i++;
	return ((unsigned char)c_1[i] - (unsigned char)c_2[i]);
}

int		time_sort(void *d1, void *d2, t_uflag flags)
{
	const time_t	diff = time_diff(d1, d2, flags);

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

void	quick_sort(void **list, int start, int end, int (*f)(void*, void*, t_uflag), t_uflag fl)
{
	void			*key;
	int				mid;
	int				i;
	int				j;

	if (!(start < end))
		return ;
	mid = (start + end) / 2;
	swap(list + start, list + mid);
	key = list[start];
	i = start + 1;
	j = end;
	while (i <= j)
	{
		while (i <= end && (fl & FLAG_REVERSE_SORT ?
		f(list[i], key, fl) >= 0 : f(list[i], key, fl) <= 0))
			i++;
		while (j >= start && (!(fl & FLAG_REVERSE_SORT) ?
		f(list[j], key, fl) > 0 : f(list[j], key, fl) < 0))
			j--;
		if (i < j)
			swap(list + i, list + j);
	}
	swap(list + start, list + j);
	quick_sort(list, start, j - 1, f, fl);
	quick_sort(list, j + 1, end, f, fl);
}

void	sort_entries(void **list, int start, int end, t_uflag flags)
{
	if (flags & FLAG_SORT_TIME_MODIFIED)
		quick_sort(list, start, end, time_sort, flags);
	else
		quick_sort(list, start, end, ft_d_name_sort, flags);
}
