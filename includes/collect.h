/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 14:22:49 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/11 14:22:49 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLECT_H
# define COLLECT_H
# include "args.h"

int		collect_entries(char **args, int len, t_uflag flags);
void	read_directory(const t_entries *entries,
	t_payload *stats, t_uflag flags, t_maxs *maximums);

#endif
