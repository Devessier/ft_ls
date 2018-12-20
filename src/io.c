/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 09:31:18 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/20 09:50:47 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include "utils.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *str, int fd)
{
	const size_t	len = ft_strlen(str);

	write(fd, str, len);
}

void	ft_putnbr_fd(int64_t n, int fd)
{
	int		pow;
	int64_t	tmp;
	int8_t	sign;

	pow = 1;
	sign = n < 0 ? -1 : 1;
	tmp = n;
	while (tmp /= 10)
		pow *= 10;
	if (sign == -1)
		ft_putchar_fd('-', fd);
	while (pow)
	{
		ft_putchar_fd(n / pow % 10 * sign + '0', fd);
		pow /= 10;
	}
}

void	ft_putchar_color_fd(char c, char *color, int fd, t_uflag flags)
{
	if (c == '-')
		return (ft_putchar_fd('-', fd));
	ft_putf_color_fd(fd, color, flags, "%c", c);
}
