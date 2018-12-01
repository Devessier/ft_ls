/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 19:29:36 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/01 17:33:06 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "list.h"
#include <errno.h>
#include "utils.h"
#include "sort.h"

t_argument	g_arguments[] =
{
	{ 'l', FLAG_LONG_FORMAT },
	{ 'R', FLAG_RECURSIVE },
	{ 'a', FLAG_INCLUDE_DOTS },
	{ 'r', FLAG_REVERSE_SORT },
	{ 't', FLAG_SORT_TIME_MODIFIED },
	{ 0, FLAG_NONE }
};

void		init_arguments(t_ls_args *self)
{
	*self = (t_ls_args) {
		.flags = 0,
		.len = 0,
		.cap = 0,
		.stats = NULL
	};
}

uint8_t		parse_flags(char *flag)
{
	uint8_t	flags;
	uint8_t	i;

	flags = FLAG_NONE;
	while (*++flag)
	{
		i = 0;
		while (g_arguments[i].c_flag)
			if (g_arguments[i++].c_flag == *flag)
				flags |= g_arguments[i - 1].flag;
	}
	return (flags);
}

void		append_arg(t_ls_args *arguments, char *path)
{
	t_stat	**tmp;
	int	i;

	if (arguments->len + 1 >= arguments->cap)
	{
		tmp = arguments->stats;
		arguments->cap = !arguments->cap ? 10 : arguments->cap * 10;
		if (!(arguments->stats = (t_stat **)malloc(sizeof(t_stat *) * arguments->cap)))
			ft_putf_fd(2, "ft_ls: %s: %s\n", path, strerror(errno));
		i = -1;
		while (++i < arguments->len)
			arguments->stats[i] = tmp[i];
		if (tmp)
			free(tmp);
	}
	if (!(arguments->stats[arguments->len] = (t_stat *)malloc(sizeof(t_stat))))
		ft_putf_fd(2, "ft_ls: %s: %s\n", path, strerror(errno));
	if (stat(path, (struct stat *)arguments->stats[arguments->len]) != 0)
		ft_putf_fd(2, "ft_ls: %s: %s\n", path, strerror(errno));
	arguments->stats[arguments->len++]->name = path;
}

t_ls_args	parse_args(int len, char **args)
{
	uint8_t		end_of_flags;
	t_ls_args	arguments;
	int			i;

	init_arguments(&arguments);
	end_of_flags = 0;
	i = 0;
	while (i < len && *args[i] == '-' && !end_of_flags)
	{
		if (args[i][1] == '-')
			end_of_flags ^= 1;
		else
			arguments.flags |= parse_flags(args[i]);
		i++;
	}
	if (!(len - i))
		append_arg(&arguments, ".");
	else if (len - i > 1)
		quick_sort((void **)args, i, len - 1, ft_strcmp);
	while (i < len)
		append_arg(&arguments, args[i++]);
	return (arguments);
}
