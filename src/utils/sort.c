/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:37:11 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:46:45 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"
#include <stdlib.h>
#include <stdbool.h>

static void merge_left(void *arr[], void **l[], int *indices,
    int (*func)(void *, void *))
{
    while (indices[0] < indices[3] && indices[1] < indices[4])
    {
        if (func(l[0][indices[0]], l[1][indices[1]]) <= 0)
            arr[indices[2]++] = l[0][indices[0]++];
        else
            arr[indices[2]++] = l[1][indices[1]++];
    }
}

static void merge_rest(void *arr[], void **l[], int *indices)
{
    while (indices[0] < indices[3])
        arr[indices[2]++] = l[0][indices[0]++];
    while (indices[1] < indices[4])
        arr[indices[2]++] = l[1][indices[1]++];
}

static bool merge(void *arr[], int *sides, int mid,
    int (*func)(void *, void *))
{
    int		n[2];
    int		i;
    void	**l[2];
    int		indices[5];

    n[0] = mid - sides[0] + 1;
    n[1] = sides[1] - mid;
    l[0] = malloc(n[0] * sizeof(void *));
    if (!l[0])
        return (false);
    l[1] = malloc(n[1] * sizeof(void *));
    if (!l[1])
        return (free(l[0]), false);
    i = -1;
    while (++i < n[0])
        l[0][i] = arr[sides[0] + i];
    i = -1;
    while (++i < n[1])
        l[1][i] = arr[mid + 1 + i];
    indices[0] = 0;
    indices[1] = 0;
    indices[2] = sides[0];
    indices[3] = n[0];
    indices[4] = n[1];
    merge_left(arr, l, indices, func);
    merge_rest(arr, l, indices);
    return (free(l[0]), free(l[1]),true);
}

bool merge_sort(void *arr[], const int left, const int right,
    int (*func)(void *, void *))
{
    int mid;
    int sides[2];

    sides[0] = left;
    sides[1] = right;
    if (left < right)
    {
        mid = left + (right - left) / 2;
        if (!merge_sort(arr, left, mid, func))
            return (false);
        if (!merge_sort(arr, mid + 1, right, func))
            return (false);
        if (!merge(arr, sides, mid, func))
            return (false);
    }
    return (true);
}
