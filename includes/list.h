/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 11:46:25 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/10 16:04:16 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H
# include <dirent.h>
# include <stdint.h>
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
# define COLOR_FILE_TYPE "\033[38;2;63;127;191m"
# define COLOR_S "\033[38;2;216;27;96m"
# define COLOR_T "\033[38;2;255;241;118m"
# define COLOR_UID "\033[30;101m"
# define COLOR_GID "\033[30;106m"
# define S_AEXEC (S_IXUSR | S_IXGRP | S_IXOTH)
# define MONTH 3600 * 24 * 30.5

typedef struct		s_maxs
{
	unsigned int	links;
	unsigned int	links_len;
	unsigned int	user;
	unsigned int	group;
	unsigned int	size;
	unsigned int	size_len;
	unsigned int	major;
	unsigned int	major_len;
	unsigned int	minor;
	unsigned int	minor_len;
	unsigned int	blocks;
}					t_maxs;

typedef struct		s_file_type
{
	uint32_t	mode;
	char		*color;
	char		to_char;
}					t_file_type;

void				list_argument(t_payload *argstat, t_uflag flags);
int					error(char *path);
void				usage(char c);
extern t_file_type	g_file_types[];

#endif
