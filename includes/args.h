/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 19:22:43 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/29 20:13:15 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include <stdint.h>
# include <dirent.h>

typedef enum	e_flag
{
	FLAG_LONG_FORMAT	= 1,
	FLAG_RECURSIVE		= 1 << 1,
	FLAG_INCLUDE_DOTS	= 1 << 2,
	FLAG_REVERSE_SORT	= 1 << 3,
	FLAG_SORT_TIME_MODIFIED = 1 << 4
}		t_flag;

typedef struct	s_entry
{
	DIR	*dirp;
}		t_entry;

typedef struct	s_args
{
	uint8_t		flags;
	t_entry		*entries;
}		t_args;

t_args		parse_args(int len, char **args);

#endif
