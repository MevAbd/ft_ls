#include "ft_ls.h"
#include <errno.h>
#include <string.h>

/*
 * Display the target of a symbolic link
 * Builds the full path of the link, reads target with readlink,
 * and displays " -> target" after the link name
 */
static void	display_symlink_target(const t_entry *entry, const char *path)
{
	char *full;
	char link_target[256];
	int link_len;
	size_t base_len;
	size_t name_len;
	size_t k;
	size_t m;
	int need_slash;

	base_len = ft_strlen(path);
	name_len = ft_strlen(entry->name);
	need_slash = (base_len > 0 && path[base_len - 1] != '/');
	full = (char *)malloc(base_len + need_slash + name_len + 1);
	if (!full)
		return;
	k = 0;
	while (k < base_len)
	{
		full[k] = path[k];
		k++;
	}
	if (need_slash)
		full[k++] = '/';
	m = 0;
	while (entry->name[m])
	{
		full[k + m] = entry->name[m];
		m++;
	}
	full[k + m] = '\0';
	link_len = readlink(full, link_target, sizeof(link_target) - 1);
	free(full);
	if (link_len >= 0)
	{
		link_target[link_len] = '\0';
		write(1, " -> ", 4);
		write(1, link_target, link_len);
	}
}

/*
 * Display a single line in long format (-l)
 * Format: "-rwxr-xr-x  1 user  group  1234  10 nov 15:00  filename"
 * Retrieves all info (type, permissions, nlink, user, group, size, date)
 * Displays target if it's a symbolic link
 */
static void	display_long_entry(const t_entry *entry, const char *path)
{
	char type;
	char perms[10];
	char nlink_buf[32];
	char size_buf[32];
	char date_buf[13];
	const char *user_name;
	const char *group_name;
	int nlink_len;
	int size_len;

	get_file_type_char(entry->st.st_mode, &type);
	get_permissions(entry->st.st_mode, perms);
	nlink_len = ft_itoa((int)entry->st.st_nlink, nlink_buf);
	get_user_info(entry->st.st_uid, &user_name);
	get_group_info(entry->st.st_gid, &group_name);
	size_len = ft_itoa((int)entry->st.st_size, size_buf);
	format_date(entry->st.st_mtime, date_buf);
	write(1, &type, 1);
	write(1, perms, 9);
	write(1, " ", 1);
	write(1, nlink_buf, nlink_len);
	write(1, " ", 1);
	write(1, user_name, ft_strlen(user_name));
	write(1, "  ", 2);
	write(1, group_name, ft_strlen(group_name));
	write(1, "  ", 2);
	write(1, size_buf, size_len);
	write(1, " ", 1);
	write(1, date_buf, ft_strlen(date_buf));
	write(1, " ", 1);
	write(1, entry->name, ft_strlen(entry->name));
	if (S_ISLNK(entry->st.st_mode))
		display_symlink_target(entry, path);
	write(1, "\n", 1);
}

/*
 * Display all entries in long format (-l)
 * Displays "total" if show_total is true (for directories only)
 * Iterates through all entries and displays each with display_long_entry
 * Handles order (normal or reverse according to flags->r)
 */
static void	display_long_format(t_entry *entries, int count, t_flags *flags,
	const char *path, int show_total)
{
	int i;

	if (show_total && count > 0)
		print_total(entries, count);
	if (!flags->r)
	{
		i = 0;
		while (i < count)
		{
			display_long_entry(&entries[i], path);
			i++;
		}
	}
	else
	{
		i = count - 1;
		while (i >= 0)
		{
			display_long_entry(&entries[i], path);
			i--;
		}
	}
}

/*
 * Display all entries in simple format (without -l)
 * Displays just the name of each file, one per line
 * Handles order (normal or reverse according to reverse)
 */
static void	display_simple_format(t_entry *entries, int count, int reverse)
{
	int i;

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

/*
 * Main entry point for display
 * Chooses between long format (-l) or simple format according to flags->l
 * show_total: 1 for directories (displays "total"), 0 for files
 */
void	display_entries(t_entry *entries, int count, t_flags *flags,
	const char *path, int show_total)
{
	if (flags->l)
		display_long_format(entries, count, flags, path, show_total);
	else
		display_simple_format(entries, count, flags->r);
}
