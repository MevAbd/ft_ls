/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malbrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 11:25:20 by malbrand          #+#    #+#             */
/*   Updated: 2025/11/15 11:25:55 by malbrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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

void	free_operands(t_entry *files, int file_count, char **dirs,
	int dir_count)
{
	int	i;

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
