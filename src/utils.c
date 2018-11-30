/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 14:57:05 by bdevessi          #+#    #+#             */
/*   Updated: 2018/11/30 16:40:39 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdarg.h>
#include <unistd.h>

size_t		ft_strlen(char *str)
{
	size_t	len;

	len = 0;
	while (*str++)
		len++;
	return (len);
}

char		*ft_strcat(char *s1, const char *s2)
{
	char *tmp;

	tmp = s1;
	while (*tmp)
		tmp++;
	while (*s2)
		*tmp++ = *s2++;
	*tmp = '\0';
	return (s1);
}

static int	is_special_char(char c)
{
	return (c == 'c' || c == 'd' || c == 's' || c == '%');
}

void		ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void		ft_putstr_fd(char *str, int fd)
{
	const size_t	len = ft_strlen(str);

	write(fd, str, len);
}

void		ft_putendl_fd(char *str, int fd)
{
	ft_putstr_fd(str, fd);
	ft_putchar_fd('\n', fd);
}

void    ft_putnbr_fd(int n, int fd)
{
	int             pow;
	int             tmp;
	char    sign;

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

void		ft_putf_fd(int fd, const char *format, ...)
{
	va_list	args;

	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			if (format[1] == 'c')
				ft_putchar_fd((char)va_arg(args, int), fd);
			else if (format[1] == 'd')
				ft_putnbr_fd(va_arg(args, int), fd);
			else if (format[1] == 's')
				ft_putstr_fd(va_arg(args, char *), fd);
			else
				ft_putchar_fd(*format, fd);
			format = format + (is_special_char(format[1]) ? 2 : 1);
		}
		else
			ft_putchar_fd(*format++, fd);
	}
	va_end(args);
}
