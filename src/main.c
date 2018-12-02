/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 20:03:51 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/02 01:26:01 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"
#include "list.h"
#include <stdio.h>
#include <sys/stat.h>
#include "utils.h"

int	main(int argc, char **argv)
{
	const	t_entries	arguments = parse_args(--argc, ++argv);
	int	i;

	i = 0;
	while (i < arguments.len)
	{
		if (arguments.len > 1 && (arguments.stats[i]->st_mode))
			ft_putf_fd(1, "%s:\n", arguments.stats[i]->d_shname);
		list_argument(arguments.stats[i], arguments.flags);
		if (arguments.len - i++ > 1)
			ft_putchar_fd('\n', 1);
	}
}
