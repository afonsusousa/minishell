//
// Created by wlucas-f on 10/23/25.
//

#ifndef MINISHELL_UTILS_H
#define MINISHELL_UTILS_H

int count_words(const char* str, const char sep);
void free_until_null(char*** str_v);
char* strjoin_three(char* s1, char* s2, char* s3);
char **strjoinjoin(char **a, char **b);
char    *ft_strndup(const char *str, size_t size);

#endif //MINISHELL_UTILS_H