/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 19:29:36 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/29 20:14:36 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t		parse_flags(char *flag)
{
	uint8_t	flags;

	flags = 0;
	flag++;
	while (*flag)
	{
		if (*flag == 'l')
			flags |= FLAG_LONG_FORMAT;
		else if (*flag == 'R')
			flags |= FLAG_RECURSIVE;
		else if (*flag == 'a')
			flags |= FLAG_INCLUDE_DOTS;
		else if (*flag == 'r')
			flags |= FLAG_REVERSE_SORT;
		else if (*flag == 't')
			flags |= FLAG_SORT_TIME_MODIFIED;
		else
			printf("Error in flag\n");
		flag++;
	}
	return (flags);
}

t_args		parse_args(int len, char **args)
{
	int	entries_count;
	uint8_t	end_of_flags;
	uint8_t	flags;
	int	i;

	entries_count = 0;
	end_of_flags = 0;
	flags = 0;
	i = 0;
	while (i < len)
	{
		printf("len = %d | arg = %s\n", len, args[i]);
		if (!end_of_flags
			&& ((*args[i] == '-' && args[i][1] == '-')
			|| (*args[i] != '-' && !end_of_flags)))
			end_of_flags ^= 1;
		if (!end_of_flags)
			flags |= parse_flags(args[i]);
		else
		{
			// arg is a string representing a file/dir/…
		}
		i++;
	}
	return ((t_args){ flags, NULL });
}
