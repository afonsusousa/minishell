//
// Created by afonsusousa on 11/20/25.
//

#include <stdlib.h>
#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"

static void	merge_left(char *arr[], char **l, char **r, int *indices)
{
    while (indices[0] < indices[3] && indices[1] < indices[4])
    {
        if (ft_strcmp(l[indices[0]], r[indices[1]]) <= 0)
            arr[indices[2]++] = l[indices[0]++];
        else
            arr[indices[2]++] = r[indices[1]++];
    }
}

static void	merge_rest(char *arr[], char **l, char **r, int *indices)
{
    while (indices[0] < indices[3])
        arr[indices[2]++] = l[indices[0]++];
    while (indices[1] < indices[4])
        arr[indices[2]++] = r[indices[1]++];
}

static bool	merge(char *arr[], int left, int mid, int right)
{
    int		n[2];
    int		i;
    char	**l;
    char	**r;
    int		indices[5];

    n[0] = mid - left + 1;
    n[1] = right - mid;
    l = malloc(n[0] * sizeof(char *));
    if (!l)
        return (false);
    r = malloc(n[1] * sizeof(char *));
    if (!r)
        return (free(l), false);
    i = -1;
    while (++i < n[0])
        l[i] = arr[left + i];
    i = -1;
    while (++i < n[1])
        r[i] = arr[mid + 1 + i];
    ft_memset(&indices, 0, 2 * sizeof(int));
    indices[2] = left;
    indices[3] = n[0];
    indices[4] = n[1];
    merge_left(arr, l, r, indices);
    merge_rest(arr, l, r, indices);
    return (free(l), free(r), true);
}

bool	merge_sort_strings(char *arr[], const int left, const int right)
{
    int	mid;

    if (left < right)
    {
        mid = left + (right - left) / 2;
        if (!merge_sort_strings(arr, left, mid))
            return (false);
        if (!merge_sort_strings(arr, mid + 1, right))
            return (false);
        if (!merge(arr, left, mid, right))
            return (false);
    }
    return (true);
}