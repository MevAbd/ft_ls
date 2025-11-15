/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_display.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malbrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 11:17:42 by malbrand          #+#    #+#             */
/*   Updated: 2025/11/15 11:20:14 by malbrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include <errno.h>
#include <string.h>

static void		display_symlink_target(const t_entry *entry, const char *path)
{
	char		*full;
	char		link_target[256];
	int		link_len;

	full = ft_build_full_path(path, entry->name);
	if (!full)
		return ;
	link_len = readlink(full, link_target, sizeof(link_target) - 1);
	free(full);
	if (link_len >= 0)
	{
		link_target[link_len] = '\0';
		write(1, " -> ", 4);
		write(1, link_target, link_len);
	}
}

static void		display_type_and_perms(mode_t mode)
{
	char		type;
	char		perms[10];

	get_file_type_char(mode, &type);
	get_permissions(mode, perms);
	write(1, &type, 1);
	write(1, perms, 9);
	write(1, " ", 1);
}

static void		display_number_padded(long long n, int max_width)
{
	char		buf[32];
	int		len;

	len = ft_putnbr(n, buf);
	write_padding(len, max_width);
	write(1, buf, len);
}

static void		display_string_padded(const char *str, int max_width)
{
	int		len;

	len = ft_strlen(str);
	write(1, str, len);
	write_padding(len, max_width);
}

static void		display_long_entry(const t_entry *entry, const char *path, int *widths)
{
	char		date_buf[13];
	const char	*user_name;
	const char	*group_name;

	display_type_and_perms(entry->st.st_mode);
	display_number_padded((long long)entry->st.st_nlink, widths[0]);
	write(1, " ", 1);
	get_user_info(entry->st.st_uid, &user_name);
	display_string_padded(user_name, widths[1]);
	write(1, " ", 1);
	get_group_info(entry->st.st_gid, &group_name);
	display_string_padded(group_name, widths[2]);
	write(1, " ", 1);
	display_number_padded((long long)entry->st.st_size, widths[3]);
	write(1, " ", 1);
	format_date(entry->st.st_mtime, date_buf);
	write(1, date_buf, ft_strlen(date_buf));
	write(1, " ", 1);
	write(1, entry->name, ft_strlen(entry->name));
	if (S_ISLNK(entry->st.st_mode))
		display_symlink_target(entry, path);
	write(1, "\n", 1);
}

static void		display_long_format(t_entry *entries, int count, t_flags *flags, const char *path, int show_total)
{
	int			i;
	int			widths[4];

	if (show_total)
		print_total(entries, count);
	if (count == 0)
		return;
	calculate_column_widths(entries, count, widths);
	if (!flags->r)
	{
		i = 0;
		while (i < count)
		{
			display_long_entry(&entries[i], path, widths);
			i++;
		}
	}
	else
	{
		i = count - 1;
		while (i >= 0)
		{
			display_long_entry(&entries[i], path, widths);
			i--;
		}
	}
}

static void		display_simple_format(t_entry *entries, int count, int reverse)
{
	int 		i;

	if (!reverse)
	{
		i = 0;
		while (i < count)
		{
			write(1, entries[i].name, ft_strlen(entries[i].name));
			write(1, "\n", 1);
			i++;
		}
	}
	else
	{
		i = count - 1;
		while (i >= 0)
		{
			write(1, entries[i].name, ft_strlen(entries[i].name));
			write(1, "\n", 1);
			i--;
		}
	}
}

void			display_entries(t_entry *entries, int count, t_flags *flags,
				const char *path, int show_total)
{
	if (flags->l)
		display_long_format(entries, count, flags, path, show_total);
	else
		display_simple_format(entries, count, flags->r);
}
