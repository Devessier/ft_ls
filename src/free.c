/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 10:50:33 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/20 10:50:34 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_ls.h"

uint8_t		free_grp_usr(char *grp, char *usr)
{
	if (grp)
		free(grp);
	if (usr)
		free(usr);
	return (1);
}

void	free_stats(t_payload *stats, t_uflag flags, bool free_names, bool link)
{
	if (free_names)
	{
		free(stats->d_shname);
		free(stats->d_name);
	}
	if (flags & FLAG_LONG_FORMAT)
	{
		free(stats->user);
		free(stats->group);
	}
	if (link && flags & FLAG_LONG_FORMAT)
		free(stats->link);
	free(stats);
}

void	free_entries(t_entries *entries, t_uflag flags)
{
	int	i;

	i = 0;
	while (i++ < entries->len)
		free_stats(entries->payloads[i - 1], flags, 1,
			S_ISLNK(entries->payloads[i - 1]->stats.st_mode));
	if (entries->payloads)
		free(entries->payloads);
}
