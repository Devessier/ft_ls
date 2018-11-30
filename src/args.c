/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 19:29:36 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/30 10:19:52 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

t_argument	g_arguments[] =
{
	{ 'l', FLAG_LONG_FORMAT },
	{ 'R', FLAG_RECURSIVE },
	{ 'a', FLAG_INCLUDE_DOTS },
	{ 'r', FLAG_REVERSE_SORT },
	{ 't', FLAG_SORT_TIME_MODIFIED },
	{ 0, FLAG_NONE }
};

uint8_t		parse_flags(char *flag)
{
	uint8_t	flags;
	uint8_t	i;

	flags = 0;
	while (*++flag)
	{
		i = 0;
		while (g_arguments[i].c_flag)
			if (g_arguments[i++].c_flag == *flag)
				flags |= g_arguments[i - 1].flag;
	}
	return (flags);
}

t_args		parse_args(int len, char **args)
{
	uint8_t	end_of_flags;
	uint8_t	flags;
	int		entries_count;
	int		i;

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
