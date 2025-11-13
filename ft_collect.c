#include "ft_ls.h"
#include <errno.h>
#include <string.h>

/* Print an error message for an inaccessible directory */
static void	print_dir_error(const char *path)
{
	const char	*p1;
	const char	*p2;
	const char	*msg;

	p1 = "ls: ";
	p2 = ": ";
	msg = strerror(errno);
	write(2, p1, ft_strlen(p1));
	write(2, path, ft_strlen(path));
	write(2, p2, ft_strlen(p2));
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}


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

/*
** Free collected entries on error
** Used to clean up on failure during collection
*/
static void	free_collected_entries(t_entry *entries, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(entries[i].name);
		i++;
	}
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

/*
** Collect all entries from a directory
** Reads the directory, filters according to show_hidden, and collects metadata
** Returns 1 on success, 0 on error
*/
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

