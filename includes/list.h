/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 11:46:25 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/30 23:26:05 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H
# include <dirent.h>
# include <stdint.h>
# include "args.h"

void	list(t_entry *entry, uint8_t flags);
void	error(char *path);

#endif
