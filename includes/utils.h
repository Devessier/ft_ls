/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 14:58:02 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/02 01:02:36 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
# include <string.h>

size_t		ft_strlen(char *str);
char		*ft_strcat(char *s1, const char *s2);
void		ft_putf_fd(int fd, const char *format, ...);
void		ft_putstr_fd(char *str, int fd);
void		ft_putnbr_fd(int n, int fd);
void		ft_putchar_fd(char c, int fd);
void		ft_putendl_fd(char *str, int fd);
char		*pathjoin(char const *s1, char const *s2);

#endif
