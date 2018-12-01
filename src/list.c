/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 10:57:31 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/01 17:17:54 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "utils.h"
#include "args.h"

void	error(char *path)
{
	ft_putf_fd(2, "ft_ls: %s: %s\n", path, strerror(errno))	;
	exit(1);
}

void	list(t_entry *entry, uint8_t flags)
{
	DIR				*directory;
	struct dirent	*d;

	errno = 0;
	if (!(directory = opendir(entry->name)))
		error(entry->name);
	while ((d = readdir(directory)) != NULL)
	{
		if (*d->d_name == '.' && !(flags & FLAG_INCLUDE_DOTS))
				continue ;
		// printf("d_fileno = %llu | d_reclen = %d | d_namlen = %d | d_type = %d | d_name = %s\n", d->d_fileno, d->d_reclen, d->d_namlen, d->d_type, d->d_name);
		ft_putendl_fd(d->d_name, 1);
	}
}
