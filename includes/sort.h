/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 13:25:26 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/03 10:35:00 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SORT_H
# define SORT_H

void	quick_sort(void **list, int start, int end, int (*f)(void *, void *));
int		ft_strcmp(void *s1, void *s2);
int		ft_d_name_sort(void *d1, void *s2);
int		args_sort(void *a1, void *a2);

#endif
