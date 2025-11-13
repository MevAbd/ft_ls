#include "ft_ls.h"


/* Free memory allocated for an array of entries */
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

/* Process a single directory entry recursively */
static void	process_single_dir(const char *path, const t_entry *entry,
	t_flags *flags)
{
	char	*full_path;

	if (ft_strcmp(entry->name, ".") != 0
		&& ft_strcmp(entry->name, "..") != 0
		&& S_ISDIR(entry->st.st_mode))
	{
		full_path = ft_build_full_path(path, entry->name);
		if (full_path != NULL)
		{
			write(1, "\n", 1);
			write(1, full_path, ft_strlen(full_path));
			write(1, ":\n", 2);
			list_files(full_path, flags);
			free(full_path);
		}
	}
}

/* Process directories recursively (option -R) */
static void	process_recursive_dirs(const char *path, t_entry *entries, int count,
	t_flags *flags)
{
	int		i;

	if (!flags->r)
	{
		i = 0;
		while (i < count)
		{
			process_single_dir(path, &entries[i], flags);
			i++;
		}
	}
	else
	{
		i = count - 1;
		while (i >= 0)
		{
			process_single_dir(path, &entries[i], flags);
			i--;
		}
	}
}

/* Counts files, collects entries, sorts, displays, and handles recursion if -R */
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