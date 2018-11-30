/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 20:03:51 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/30 14:34:45 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"
#include "list.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_args	arguments = parse_args(--argc, ++argv);

	if (!arguments.entries_count)
		list(".", arguments.flags);
	while (arguments.entries_count--)
		list(*arguments.entries++, arguments.flags);
}
