/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 19:22:43 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/03 14:40:01 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGS_H
# define ARGS_H
# include <stdint.h>
# include <dirent.h>
# include <sys/stat.h>

typedef enum		e_flag
{
	FLAG_NONE = 0,
	FLAG_LONG_FORMAT = 1,
	FLAG_RECURSIVE = 1 << 1,
	FLAG_INCLUDE_DOTS = 1 << 2,
	FLAG_REVERSE_SORT = 1 << 3,
	FLAG_SORT_TIME_MODIFIED = 1 << 4,
	FLAG_COLORS_ON = 1 << 5
}					t_flag;

typedef struct		s_entry
{
	uint16_t	record_len;
	uint16_t	name_len;
	uint8_t		type;
	char		name[1024];
	struct stat	*stats;
}					t_entry;

typedef struct		s_stat
{
	dev_t			st_dev;
	mode_t			st_mode;
	nlink_t			st_nlink;
	ino_t			st_ino;
	uid_t			st_uid;
	gid_t			st_gid;
	dev_t			st_rdev;
	struct timespec	st_atimespec;
	struct timespec	st_mtimespec;
	struct timespec	st_ctimespec;
	struct timespec	st_birthtimespec;
	off_t			st_size;
	blkcnt_t		st_blocks;
	blksize_t		st_blksize;
	uint32_t		st_flags;
	uint32_t		st_gen;
	uint8_t			d_type;
	char			*d_name;
	char			*d_shname;
}					t_stat;

typedef struct		s_entries
{
	uint8_t	flags;
	int		cap;
	int		len;
	t_stat	**stats;
}					t_entries;

typedef struct		s_argument
{
	char	c_flag;
	t_flag	flag;
}					t_argument;

extern t_argument	g_arguments[];

t_entries			parse_args(int len, char **args);
int					append_entry(t_entries *entries, char *long_name,
		char *short_name, uint8_t watch_sym_link);

#endif
