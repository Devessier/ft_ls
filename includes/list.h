/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 11:46:25 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/07 14:45:09 by bdevessi         ###   ########.fr       */
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

typedef struct		s_maxs
{
	unsigned int	links;
	unsigned int	user;
	unsigned int	group;
	unsigned int	size;
	unsigned int	blocks;
}					t_maxs;

typedef struct		s_file_type
{
	uint32_t	mode;
	char		*color;
	char		to_char;
}					t_file_type;

void				list_argument(t_payload *argstat, uint8_t flags);
int					error(char *path);
extern t_file_type	g_file_types[];

#endif
