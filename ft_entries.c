#include "ft_ls.h"
#include <errno.h>
#include <string.h>


/* Collect data for a single entry (file/directory) */
static int		collect_single_entry(const char *path, const char *name, t_entry *entry)
{
	char		*full;
	struct stat	st;

	full = ft_build_full_path(path, name);
	if (full == NULL)
		return (0);
	if (lstat(full, &st) != 0)
	{
		free(full);
		return (0);
	}
	free(full);
	entry->name = ft_strdup(name);
	if (entry->name == NULL)
		return (0);
	entry->mtime = st.st_mtime;
	entry->st = st;
	return (1);
}


/* Count the number of files in a directory */
int					count_files(const char *path, int show_hidden)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	count = 0;
	dir = opendir(path);
	if (dir == NULL)
	{
		print_dir_error(path);
		return (0);
	}
	while ((entry = readdir(dir)) != NULL)
	{
		if (show_hidden || entry->d_name[0] != '.')
			count++;
	}
	closedir(dir);
	return (count);
}

/* Collect all entries from a directory */
int	collect_entries(const char *path, t_entry *entries, int count,
	int show_hidden)
{
	DIR				*dir;
	struct dirent	*entry;
	int				i;

	dir = opendir(path);
	if (dir == NULL)
		return (0);
	i = 0;
	while ((entry = readdir(dir)) != NULL && i < count)
	{
		if (show_hidden || entry->d_name[0] != '.')
		{
			if (!collect_single_entry(path, entry->d_name, &entries[i]))
			{
				free_collected_entries(entries, i);
				closedir(dir);
				return (0);
			}
			i++;
		}
	}
	closedir(dir);
	return (1);
}