/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 19:22:43 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/13 11:15:45 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGS_H
# define ARGS_H
# include <stdint.h>
# include <dirent.h>
# include <sys/stat.h>
# include <pwd.h>
# include <grp.h>

typedef uint16_t	t_uflag;

typedef enum		e_flag
{
	FLAG_NONE = 0,
	FLAG_LONG_FORMAT = 1,
	FLAG_RECURSIVE = 1 << 1,
	FLAG_INCLUDE_DOTS = 1 << 2,
	FLAG_REVERSE_SORT = 1 << 3,
	FLAG_SORT_TIME_MODIFIED = 1 << 4,
	FLAG_COLORS_ON = 1 << 5,
	FLAG_NUMERIC = 1 << 6,
	FLAG_LAST_ACCESS = 1 << 7,
	FLAG_CREATION = 1 << 8,
	FLAG_STATUS_CHANGED = 1 << 9,
}					t_flag;

typedef struct		s_payload
{
	struct stat	stats;
	char		*user;
	char		*group;
	char		*d_name;
	char		*d_shname;
	char		*link;
}					t_payload;

typedef struct		s_entries
{
	t_uflag		flags;
	int			cap;
	int			len;
	t_payload	**payloads;
}					t_entries;

typedef struct		s_argument
{
	char	c_flag;
	t_flag	flag;
}					t_argument;

extern t_argument	g_arguments[];

void				parse_args(int len, char **args);
int					append_entry(t_entries *entries, struct stat stats,
	char *long_name, char *short_name);

#endif
