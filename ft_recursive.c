#include "ft_ls.h"

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
void	process_recursive_dirs(const char *path, t_entry *entries, int count,
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