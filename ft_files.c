#include "ft_ls.h"

/*
** Build a full path from a base directory and a name
** Automatically handles adding '/' if necessary
** Returns NULL on allocation error
*/
static char	*build_full_path(const char *base, const char *name)
{
	size_t	base_len;
	size_t	name_len;
	size_t	k;
	size_t	m;
	int		need_slash;
	char	*full;

	base_len = ft_strlen(base);
	name_len = ft_strlen(name);
	need_slash = (base_len > 0 && base[base_len - 1] != '/');
	full = (char *)malloc(base_len + need_slash + name_len + 1);
	if (full == NULL)
		return (NULL);
	k = 0;
	while (k < base_len)
	{
		full[k] = base[k];
		k++;
	}
	if (need_slash)
		full[k++] = '/';
	m = 0;
	while (m < name_len)
	{
		full[k + m] = name[m];
		m++;
	}
	full[k + m] = '\0';
	return (full);
}

/*
** Free memory allocated for an array of entries
** Frees each name, then the array itself
*/
void	free_entries(t_entry *entries, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(entries[i].name);
		i++;
	}
	free(entries);
}

/*
** Process directories recursively (option -R)
** Iterates through all entries, and for each directory (except . and ..),
** displays the path then recursively lists its content
** If flags->r is true, processes directories in reverse order
*/
static void	process_recursive_dirs(const char *path, t_entry *entries, int count,
	t_flags *flags)
{
	int		i;
	char	*full_path;

	if (!flags->r)
	{
		i = 0;
		while (i < count)
		{
			if (ft_strcmp(entries[i].name, ".") != 0
				&& ft_strcmp(entries[i].name, "..") != 0
				&& S_ISDIR(entries[i].st.st_mode))
			{
				full_path = build_full_path(path, entries[i].name);
				if (full_path != NULL)
				{
					write(1, "\n", 1);
					write(1, full_path, ft_strlen(full_path));
					write(1, ":\n", 2);
					list_files(full_path, flags);
					free(full_path);
				}
			}
			i++;
		}
	}
	else
	{
		i = count - 1;
		while (i >= 0)
		{
			if (ft_strcmp(entries[i].name, ".") != 0
				&& ft_strcmp(entries[i].name, "..") != 0
				&& S_ISDIR(entries[i].st.st_mode))
			{
				full_path = build_full_path(path, entries[i].name);
				if (full_path != NULL)
				{
					write(1, "\n", 1);
					write(1, full_path, ft_strlen(full_path));
					write(1, ":\n", 2);
					list_files(full_path, flags);
					free(full_path);
				}
			}
			i--;
		}
	}
}

/*
** List files in a directory
** Counts files, collects entries, sorts, displays, and handles recursion if -R
** Main entry point for listing a directory
*/
void	list_files(const char *path, t_flags *flags)
{
	t_entry *entries;
	int count;
	int show_hidden;

	show_hidden = flags->a;
	count = count_files(path, show_hidden);
	if (count == 0)
		return;
	entries = (t_entry *)malloc(sizeof(t_entry) * count);
	if (entries == NULL)
		return;
	if (!collect_entries(path, entries, count, show_hidden))
	{
		free(entries);
		return;
	}
	sort_entries(entries, count, flags->t);
	display_entries(entries, count, flags, path, 1);
	if (flags->R)
		process_recursive_dirs(path, entries, count, flags);
	free_entries(entries, count);
}