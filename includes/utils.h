/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:40:10 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 18:34:15 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdbool.h>
# include <stddef.h>

// Forward declaration
typedef struct s_word	t_word;

bool					is_space(char c);
bool					is_slash(char c);
void					free_until_null(char ***str_v);
char					*strjoin_three(char *s1, char *s2, char *s3);
char					**str_arr_append(char **arr, const char *s);
char					*ft_strndup(const char *str, size_t size);
char					*ft_strnjoin(const char *s1, const char *s2,
							size_t scnd);
bool					is_valid(char c);
int						ft_strcmp(const char *s1, const char *s2);
bool					merge_sort_strings(char *arr[], int left, int right);
bool					has_char_fn(const char *s, bool (*predicate)(char));

#endif // UTILS_H
