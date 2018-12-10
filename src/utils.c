/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 14:57:05 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/10 16:06:45 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

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

void	ft_putstr_color_fd(char c, char *color, int fd, t_uflag flags)
{
	if (c == '-')
		return (ft_putchar_fd('-', fd));
	ft_putf_fd(fd, "%s%c%s", flags & FLAG_COLORS_ON ? color : ""
			, c, flags & FLAG_COLORS_ON ? COLOR_RESET : "");
}

char	*pathjoin(char *s1, char *s2)
{
	char			*str;
	unsigned int		i;
	unsigned int		j;
	size_t			len;

	if (!(s1 && s2))
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	if (!(str = (char *)malloc(len + 1)))
		return (NULL);
	while (len)
		str[len--] = 0;
	i = 0;
	while (i[s1])
		*str++ = s1[i++];
	if (str[-1] != '/')
	{
		*str++ = '/';
		i++;
	}
	j = 0;
	while (j[s2])
		*str++ = s2[j++];
	return (str - i - j);
}

size_t		nb_len(int n)
{
	size_t	i;

	i = 1;
	while (n /= 10)
		i++;
	return (i);
}

char		*ft_itoa(int n)
{
	char	*str;
	long	pow;
	int		len;
	char	sign;

	pow = 1;
	len = 1;
	sign = n < 0 ? -1 : 1;
	while (n / (pow *= 10))
		len++;
	pow /= 10;
	if (!(str = (char *)malloc(sizeof(char) * (len + (n < 0) + 1))))
		return (NULL);
	if (n < 0)
		*str++ = '-';
	while (pow)
	{
		*str++ = (n / pow % 10) * sign + '0';
		pow /= 10;
	}
	*str = '\0';
	return (str - (len + (n < 0)));
}

char	*ft_strdup(char *s1)
{
	char	*tmp;
	char	*dest;

	if (!(dest = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1))))
		return (NULL);
	tmp = dest;
	while (*s1)
		*dest++ = *s1++;
	*dest = '\0';
	return (tmp);
}
