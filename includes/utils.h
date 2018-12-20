/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 14:58:02 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/20 09:47:15 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
# include <string.h>
# include <stdint.h>
# include "args.h"

size_t	ft_strlen(char *str);
void	ft_putnbr_fd(int64_t n, int fd);
void	ft_putf_fd(int fd, const char *format, ...);
void	ft_putchar_color_fd(char c, char *color, int fd, t_uflag flags);
void	ft_putf_color_fd(int fd, char *color,
	t_uflag flags, const char *format, ...);
void	ft_putstr_fd(char *str, int fd);
void	ft_putchar_fd(char c, int fd);
char	*pathjoin(char *s1, char *s2);
size_t	nb_len(uint64_t n);
char	*ft_itoa(int64_t nb);
char	*ft_strdup(char *s1);

#endif
