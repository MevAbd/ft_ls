#include "ft_ls.h"

/* Calculate the length of a string */
size_t			ft_strlen(const char *s)
{
	size_t		len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

/* Compare two strings (case-sensitive) */
int				ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

/* Duplicate a string */
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

/* Convert an unsigned integer to string in a buffer */
int				ft_utoa(unsigned int n, char *buf)
{
	int		len;
	int		i;
	unsigned int	tmp;

	len = 0;
	tmp = n;
	if (tmp == 0)
	{
		buf[0] = '0';
		len = 1;
	}
	else
	{
		i = 0;
		while (tmp > 0)
		{
			buf[i++] = '0' + (tmp % 10);
			tmp /= 10;
		}
		len = i;
		i = 0;
		while (i < len / 2)
		{
			char	c;

			c = buf[i];
			buf[i] = buf[len - 1 - i];
			buf[len - 1 - i] = c;
			i++;
		}
	}
	buf[len] = '\0';
	return (len);
}