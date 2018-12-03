/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 11:46:25 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/03 12:43:59 by bdevessi         ###   ########.fr       */
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

void	list_argument(t_stat *argstat, uint8_t flags);
int	error(char *path);

#endif
