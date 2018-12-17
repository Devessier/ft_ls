/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 13:25:26 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/13 15:53:24 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SORT_H
# define SORT_H

void	quick_sort(void **list, int start, int end, int (*f)(void*, void*, t_uflag), t_uflag flags);
int		ft_strcmp(void *s1, void *s2, t_uflag flags);
int		ft_d_name_sort(void *d1, void *s2, t_uflag flags);
int		args_sort(void *a1, void *a2, t_uflag flags);
int		time_sort(void *a1, void *a2, t_uflag flags);
void	sort_entries(void **list, int start, int end, t_uflag flags);
int		time_diff(void *d1, void *d2, t_uflag flags);

#endif
