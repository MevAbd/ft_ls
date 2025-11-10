#include "ft_ls.h"

/*
 * Calculate the length of a string
 * Returns the number of characters before '\0'
 */
size_t	ft_strlen(const char *s)
{
	size_t len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

/*
 * Compare two strings (case-sensitive)
 * Returns < 0 if s1 < s2, > 0 if s1 > s2, 0 if equal
 */
int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

/*
 * Convert a character to lowercase
 * Returns the character in lowercase or unchanged
 */
static int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

/*
 * Compare two strings (case-insensitive)
 * Used for alphabetical sorting (like ls)
 * Returns < 0 if s1 < s2, > 0 if s1 > s2, 0 if equal
 */
int	ft_strcasecmp(const char *s1, const char *s2)
{
	unsigned char c1;
	unsigned char c2;

	while (*s1 && *s2)
	{
		c1 = (unsigned char)ft_tolower(*s1);
		c2 = (unsigned char)ft_tolower(*s2);
		if (c1 != c2)
			return (c1 - c2);
		s1++;
		s2++;
	}
	c1 = (unsigned char)ft_tolower(*s1);
	c2 = (unsigned char)ft_tolower(*s2);
	return (c1 - c2);
}

/*
 * Duplicate a string
 * Allocates memory and copies the string
 * Returns NULL on allocation error
 */
char	*ft_strdup(const char *s)
{
	size_t len;
	size_t i;
	char *dup;

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

/*
 * Swap two string pointers
 * Used for sorting
 */
void	ft_swap(char **a, char **b)
{
	char *tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}
