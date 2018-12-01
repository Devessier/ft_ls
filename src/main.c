/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 20:03:51 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/01 17:34:57 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"
#include "list.h"
#include <stdio.h>
#include <sys/stat.h>

int	main(int argc, char **argv)
{
	t_ls_args	arguments = parse_args(--argc, ++argv);

	/*if (!entries->len)
		list((t_entries){
			.flags = entries->flags;
			.cap = 1,
			.len = 1,
			.entries = &(t_entry)
		}, entries->flags);*/
	int	i = 0;
	while (arguments.len--)
	{
		printf("mode of %s : %d\n", arguments.stats[i]->name, arguments.stats[i]->st_mode);
		i++;
	}
	//while (entries->len--)
	//	list(entries->entries++, entries->flags);
}
