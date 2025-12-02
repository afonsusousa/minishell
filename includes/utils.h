//
// Created by wlucas-f on 10/23/25.
//

#ifndef MINISHELL_UTILS_H
#define MINISHELL_UTILS_H

#include <stddef.h>
#include <stdbool.h>

// Forward declaration
typedef struct s_word t_word;

bool    is_space(char c);
bool    is_slash(char c);
int     count_words(const char *str, const char sep);
void    free_until_null(char ***str_v);
char    *strjoin_three(char *s1, char *s2, char *s3);
char    **strjoinjoin(char **a, char **b);
char    *ft_strndup(const char *str, size_t size);
char    *ft_strnjoin(const char *s1, const char *s2, size_t scnd);
bool    is_valid(char c);
char    **get_double_from_str(const char *str);
int     ft_strcmp(const char *s1, const char *s2);
bool	merge_sort_strings(char *arr[], int left, int right);
char    *ft_strchr_pred(const char *s, bool (*predicate)(char));
bool    has_char_fn(const char *s, bool (*predicate)(char));

// t_tracked_str utilities
t_word   *word_new(const char *content, bool all_quoted);
void     word_free(t_word *ts);
t_word   **word_split(t_word *ts, bool (*is_separator)(char), bool quote_aware);
void     word_free_until_null(t_word **splits);
t_word   *word_dup(const t_word *ts);
t_word   **word_array_join(t_word **a, t_word **b);
t_word   **word_array_from_word(t_word *word);
char     *word_to_cstr(const t_word *word);
char     **word_to_cstr_array(t_word **words);

#endif //MINISHELL_UTILS_H
