/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdevessi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 14:57:05 by bdevessi          #+#    #+#             */
/*   Updated: 2018/12/20 09:53:19 by bdevessi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdint.h>
#include "ft_ls.h"

char	*pathjoin(char *s1, char *s2)
{
	char				*str;
	unsigned int		i;
	unsigned int		j;
	size_t				len;

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

size_t	nb_len(uint64_t n)
{
	size_t	i;

	i = 1;
	while (n /= 10)
		i++;
	return (i);
}

char	*ft_itoa(int64_t n)
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

char	*color_code(mode_t st_mode, t_uflag flags)
{
	uint8_t			i;

	if (!(flags & FLAG_COLORS_ON))
		return ("");
	if (S_ISDIR(st_mode) && (st_mode & S_IWUSR
			|| st_mode & S_IWGRP) && st_mode & S_IWOTH)
		return (st_mode & S_ISVTX ? COLOR_SUPER_DIR_SAVE : COLOR_SUPER_DIR);
	if (S_ISREG(st_mode) && st_mode & S_ISUID && st_mode & S_AEXEC)
		return (COLOR_UID);
	if (S_ISREG(st_mode) && st_mode & S_ISGID && st_mode & S_AEXEC)
		return (COLOR_GID);
	i = 0;
	while (g_file_types[i].mode)
		if ((st_mode & S_IFMT) == g_file_types[i++].mode
				&& *g_file_types[i - 1].color)
			return (g_file_types[i - 1].color);
	return (st_mode & S_IXUSR ? COLOR_EXEC : "");
}
