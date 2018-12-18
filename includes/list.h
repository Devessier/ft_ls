/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 11:46:25 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/14 11:01:38 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H
# include <dirent.h>
# include <stdint.h>
# include <sys/types.h>
# include <stdbool.h>
# include "args.h"
# define COLOR_RESET "\033[0m"
# define COLOR_FIFO "\033[33m"
# define COLOR_CHR "\033[34;103m"
# define COLOR_BLK "\033[34;106m"
# define COLOR_DIR "\033[36m"
# define COLOR_LNK "\033[35m"
# define COLOR_SOCK "\033[32m"
# define COLOR_EXEC "\033[31m"
# define COLOR_READ "\033[32m"
# define COLOR_WRITE "\033[38;2;255;127;80m"
# define COLOR_S "\033[38;2;216;27;96m"
# define COLOR_T "\033[38;2;255;241;118m"
# define COLOR_UID "\033[30;101m"
# define COLOR_GID "\033[30;106m"
# define COLOR_SUPER_DIR "\033[30;103m"
# define COLOR_SUPER_DIR_SAVE "\033[30;42m"
# define S_AEXEC (S_IXUSR | S_IXGRP | S_IXOTH)

typedef struct		s_maxs
{
	nlink_t		links;
	size_t		links_len;
	size_t		user;
	size_t		group;
	off_t		size;
	size_t		size_len;
	dev_t		major;
	size_t		major_len;
	dev_t		minor;
	size_t		minor_len;
	blkcnt_t	blocks;
}					t_maxs;

typedef struct		s_file_type
{
	uint32_t	mode;
	char		*color;
	char		to_char;
}					t_file_type;

void				list_argument(t_payload *argstat, t_uflag flags);
int					error(char *path, t_uflag flags);
void				usage(char c);
void				update_maximums(t_payload *payload, t_maxs *maximums);
void				calculate_max_len(t_maxs *maximums);
void				list_file(t_payload *payload,
	t_uflag flags, t_maxs *maximums);
void				free_stats(t_payload *stats, t_uflag flags, bool free_names, bool link);
extern t_file_type	g_file_types[];

#endif
