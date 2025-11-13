#include "ft_ls.h"

/* Free collected entries on error */
void	free_collected_entries(t_entry *entries, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(entries[i].name);
		i++;
	}
}

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

/* Free memory allocated for operands */
void	free_operands(t_entry *files, int file_count, char **dirs, int dir_count)
{
	int i;

	i = 0;
	while (i < file_count)
	{
		free(files[i].name);
		i++;
	}
	free(files);
	i = 0;
	while (i < dir_count)
	{
		free(dirs[i]);
		i++;
	}
	free(dirs);
}
