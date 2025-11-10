#include "ft_ls.h"
#include <errno.h>
#include <string.h>

/*
 * Compare two entries for sorting
 * If use_time is true, sorts by modification date (newest first)
 * When dates are equal, sorts alphabetically in ascending order (like ls)
 * Otherwise, sorts by name (case-sensitive alphabetical order, ascending)
 * With LC_ALL=C, ls uses case-sensitive sorting
 * Returns < 0 if a < b, > 0 if a > b, 0 if equal
 */
static int	compare_entries(const t_entry *a, const t_entry *b, int use_time)
{
	int time_cmp;

	if (use_time)
	{
		if (a->mtime > b->mtime)
			return (-1);
		if (a->mtime < b->mtime)
			return (1);
		time_cmp = ft_strcmp(a->name, b->name);
		return (time_cmp);
	}
	return (ft_strcmp(a->name, b->name));
}

/*
 * Sort an array of entries (bubble sort)
 * If use_time is true, sorts by modification date (newest first)
 * When dates are equal, sorts alphabetically in ascending order (like ls)
 * Otherwise, sorts alphabetically by name (ascending order)
 * Note: . and .. are sorted normally with other files (like ls)
 * Note: -r option reverses the display order, not the sort order
 */
void	sort_entries(t_entry *entries, int count, int use_time, int use_a)
{
	int i;
	int j;
	t_entry tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - 1 - i)
		{
			if (compare_entries(&entries[j], &entries[j + 1], use_time) > 0)
			{
				tmp = entries[j];
				entries[j] = entries[j + 1];
				entries[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

/*
 * Print an error message for an inaccessible directory
 * Format: "ls: path: error message\n"
 */
static void	print_dir_error(const char *path)
{
	const char *p1 = "ls: ";
	const char *p2 = ": ";
	const char *msg = strerror(errno);

	write(2, p1, ft_strlen(p1));
	write(2, path, ft_strlen(path));
	write(2, p2, ft_strlen(p2));
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

/*
 * Count the number of files in a directory
 * If show_hidden is true, also counts hidden files (starting with '.')
 * Returns 0 on error (inaccessible directory)
 */
int	count_files(const char *path, int show_hidden)
{
	DIR *dir;
	struct dirent *entry;
	int count;

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
 * Build a full path from a base directory and a name
 * Automatically handles adding '/' if necessary
 * Example: base="/home/user", name="file" → "/home/user/file"
 * Returns NULL on allocation error
 */
static char	*build_full_path(const char *base, const char *name)
{
	size_t base_len;
	size_t name_len;
	size_t k;
	size_t m;
	int need_slash;
	char *full;

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
 * Collect metadata for a single entry (file/directory)
 * Builds the full path, does an lstat, and stores info in entry
 * Returns 1 on success, 0 on error
 */
static int	collect_single_entry(const char *path, const char *name, t_entry *entry)
{
	char *full;
	struct stat st;

	full = build_full_path(path, name);
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
 * Free collected entries on error
 * Used to clean up on failure during collection
 */
static void	free_collected_entries(t_entry *entries, int count)
{
	int i;

	i = 0;
	while (i < count)
	{
		free(entries[i].name);
		i++;
	}
}

/*
 * Collect all entries from a directory
 * Reads the directory, filters according to show_hidden, and collects metadata
 * Returns 1 on success, 0 on error
 */
int	collect_entries(const char *path, t_entry *entries, int count,
	int show_hidden)
{
	DIR *dir;
	struct dirent *entry;
	int i;

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

/*
 * Free memory allocated for an array of entries
 * Frees each name, then the array itself
 */
void	free_entries(t_entry *entries, int count)
{
	int i;

	i = 0;
	while (i < count)
	{
		free(entries[i].name);
		i++;
	}
	free(entries);
}

/*
 * Process directories recursively (option -R)
 * Iterates through all entries, and for each directory (except . and ..),
 * displays the path then recursively lists its content
 * If flags->r is true, processes directories in reverse order
 */
static void	process_recursive_dirs(const char *path, t_entry *entries, int count, t_flags *flags)
{
	int i;
	char *full_path;

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
 * List files in a directory
 * Counts files, collects entries, sorts, displays, and handles recursion if -R
 * Main entry point for listing a directory
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
	sort_entries(entries, count, flags->t, flags->a);
	display_entries(entries, count, flags, path, 1);
	if (flags->R)
		process_recursive_dirs(path, entries, count, flags);
	free_entries(entries, count);
}
