#include "ft_ls.h"

/* Calculate maximum widths for each column in long format */
void		calculate_column_widths(t_entry *entries, int count, int *widths)
{
	int			i;
	char		buf[32];
	int			len;
	const char	*user_name;
	const char	*group_name;

	widths[0] = 0; /* nlink */
	widths[1] = 0; /* user */
	widths[2] = 0; /* group */
	widths[3] = 0; /* size */
	i = 0;
	while (i < count)
	{
		len = ft_putnbr((long long)entries[i].st.st_nlink, buf);
		if (len > widths[0])
			widths[0] = len;
		get_user_info(entries[i].st.st_uid, &user_name);
		len = ft_strlen(user_name);
		if (len > widths[1])
			widths[1] = len;
		get_group_info(entries[i].st.st_gid, &group_name);
		len = ft_strlen(group_name);
		if (len > widths[2])
			widths[2] = len;
		len = ft_putnbr((long long)entries[i].st.st_size, buf);
		if (len > widths[3])
			widths[3] = len;
		i++;
	}
}

/* Write padding spaces to align columns */
void		write_padding(int current_len, int max_width)
{
	int		i;

	i = 0;
	while (i < max_width - current_len)
	{
		write(1, " ", 1);
		i++;
	}
}