#include "ft_ls.h"

/* Sort an array of directory paths alphabetically */
void		sort_dir_paths(char **dirs, int dir_count)
{
	int 	i;
	int 	j;

	i = 0;
	while (i < dir_count - 1)
	{
		j = 0;
		while (j < dir_count - 1 - i)
		{
			if (ft_strcmp(dirs[j], dirs[j + 1]) > 0)
			{
				char *tmp = dirs[j];
				dirs[j] = dirs[j + 1];
				dirs[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

/* Compare two entries for sorting */
static int	compare_entries(const t_entry *a, const t_entry *b, int use_time)
{
	long	sec_diff;
	long	nsec_diff;

	if (use_time)
	{
		sec_diff = (long)a->st.st_mtimespec.tv_sec - (long)b->st.st_mtimespec.tv_sec;
		if (sec_diff != 0)
			return (sec_diff > 0 ? -1 : 1);
		nsec_diff = (long)a->st.st_mtimespec.tv_nsec - (long)b->st.st_mtimespec.tv_nsec;
		if (nsec_diff != 0)
			return (nsec_diff > 0 ? -1 : 1);
		return (ft_strcmp(a->name, b->name));
	}
	return (ft_strcmp(a->name, b->name));
}

/* Sort an array of entries (bubble sort) */
void		sort_entries(t_entry *entries, int count, int use_time)
{
	int		i;
	int		j;
	t_entry	tmp;

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
