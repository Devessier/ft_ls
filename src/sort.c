/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 13:04:38 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/03 10:57:15 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <sys/stat.h>
#include "args.h"
#include "sort.h"

int		ft_strcmp(void *s1, void *s2)
{
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

int		args_sort(void *a1, void *a2)
{
	const int	is_dir1 = S_ISDIR(((t_stat *)a1)->st_mode);
	const int	is_dir2 = S_ISDIR(((t_stat *)a2)->st_mode);

	if (is_dir1 != is_dir2)
		return ((is_dir1 && !is_dir2) ? 1 : -1);
	return (ft_d_name_sort(a1, a2));
}

int		ft_d_name_sort(void *d1, void *d2)
{
	return (ft_strcmp((void *)(((t_stat *)d1)->d_name),
				(void *)(((t_stat *)d2)->d_name)));
}

void	swap(void **a, void **b)
{
	void	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	quick_sort(void **list, int start, int end, int (*f)(void*, void*))
{
	void	*key;
	int		mid;
	int		i;
	int		j;

	if (start < end)
	{
		mid = (start + end) / 2;
		swap(list + start, list + mid);
		key = list[start];
		i = start + 1;
		j = end;
		while (i <= j)
		{
			while (i <= end && f(list[i], key) <= 0)
				i++;
			while (j >= start && f(list[j], key) > 0)
				j--;
			if (i < j)
				swap(list + i, list + j);
		}
		swap(list + start, list + j);
		quick_sort(list, start, j - 1, f);
		quick_sort(list, j + 1, end, f);
	}
}
