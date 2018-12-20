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

void	quick_sort(void **list, t_sort_args args,
	int (*f)(void*, void*, t_uflag))
{
	const int	mid = (args.start + args.end) / 2;
	int			i;
	int			j;

	if (!(args.start < args.end))
		return ;
	swap(list + args.start, list + mid);
	i = args.start + 1;
	j = args.end;
	while (i <= j)
	{
		while (i <= args.end && (args.fl & FLAG_REVERSE_SORT ?
			f(list[i], list[args.start], args.fl) >= 0 :
			f(list[i], list[args.start], args.fl) <= 0))
			i++;
		while (j >= args.start && (!(args.fl & FLAG_REVERSE_SORT) ?
			f(list[j], list[args.start], args.fl) > 0 :
			f(list[j], list[args.start], args.fl) < 0))
			j--;
		if (i < j)
			swap(list + i, list + j);
	}
	swap(list + args.start, list + j);
	quick_sort(list, (t_sort_args) { args.fl, args.start, j - 1 }, f);
	quick_sort(list, (t_sort_args) { args.fl, j + 1, args.end }, f);
}
