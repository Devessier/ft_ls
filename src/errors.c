/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 10:00:43 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/20 10:00:49 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ft_ls.h"

uint8_t	error(char *path, t_uflag flags)
{
	ft_putf_fd(2, "ft_ls: %s: ", path);
	ft_putf_color_fd(2, COLOR_EXEC, flags, "%s", strerror(errno));
	ft_putchar_fd('\n', 2);
	errno = 0;
	return (1);
}

void	usage(char c)
{
	uint8_t	i;

	i = 0;
	ft_putf_fd(2, "ft_ls: illegal option -- %c\n", c);
	ft_putstr_fd("usage: ft_ls [-", 2);
	while (g_arguments[i].c_flag)
		ft_putchar_fd(g_arguments[i++].c_flag, 2);
	ft_putstr_fd("] [file ...]\n", 2);
	exit(1);
}
