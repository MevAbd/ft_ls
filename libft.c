/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malbrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 11:37:48 by malbrand          #+#    #+#             */
/*   Updated: 2025/11/15 11:38:01 by malbrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

size_t			ft_strlen(const char *s)
{
	size_t		len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

int				ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

char			*ft_strdup(const char *s)
{
	size_t 		len;
	size_t 		i;
	char 		*dup;

	len = ft_strlen(s);
	dup = (char *)malloc(sizeof(char) * (len + 1));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[len] = '\0';
	return (dup);
}

int				ft_putnbr(long long n, char *buf)
{
	int			len;
	int			i;
	int			neg;
	long long	tmp;

	len = 0;
	neg = (n < 0);
	if (neg)
		n = -n;
	if (n == 0)
	{
		buf[0] = '0';
		return (1);
	}
	tmp = n;
	while (tmp > 0)
	{
		buf[len++] = '0' + (tmp % 10);
		tmp /= 10;
	}
	i = 0;
	while (i < len / 2)
	{
		char	c;

		c = buf[i];
		buf[i] = buf[len - 1 - i];
		buf[len - 1 - i] = c;
		i++;
	}
	if (neg)
	{
		i = len;
		while (i > 0)
		{
			buf[i] = buf[i - 1];
			i--;
		}
		buf[0] = '-';
		len++;
	}
	buf[len] = '\0';
	return (len);
}
