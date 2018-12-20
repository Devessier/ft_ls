/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 09:38:33 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/20 09:47:38 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stddef.h>
#include "utils.h"
#include "list.h"

static int	is_special_char(char c)
{
	return (c == 'c' || c == 'd' || c == 's' || c == '%');
}

size_t	ft_strlen(char *str)
{
	size_t	len;

	len = 0;
	while (*str++)
		len++;
	return (len);
}

void	ft_putf_va(int fd, const char *format, va_list args)
{
	while (*format)
	{
		if (*format == '%')
		{
			if (format[1] == 'c')
				ft_putchar_fd((char)va_arg(args, int), fd);
			else if (format[1] == 'd')
				ft_putnbr_fd(va_arg(args, int64_t), fd);
			else if (format[1] == 's')
				ft_putstr_fd(va_arg(args, char *), fd);
			else
				ft_putchar_fd(*format, fd);
			format = format + (is_special_char(format[1]) ? 2 : 1);
		}
		else
			ft_putchar_fd(*format++, fd);
	}
}

void	ft_putf_fd(int fd, const char *format, ...)
{
	va_list		args;

	va_start(args, format);
	ft_putf_va(fd, format, args);
	va_end(args);
}

void	ft_putf_color_fd(int fd, char *color,
		t_uflag flags, const char *format, ...)
{
	va_list		args;

	va_start(args, format);
	if (flags & FLAG_COLORS_ON)
		ft_putstr_fd(color, fd);
	ft_putf_va(fd, format, args);
	if (flags & FLAG_COLORS_ON)
		ft_putstr_fd(COLOR_RESET, fd);
	va_end(args);
}
