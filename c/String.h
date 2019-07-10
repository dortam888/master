/* This is a header for reimplementation of some of string.h functions */
#include <stdio.h> /*size_t*/

size_t My_strlen(const char *str);

int My_strcmp(const char *str1, const char *str2);

char *My_strcpy(char *destination, const char *source);

char *My_strncpy(char *destination, const char *source, size_t num);

int My_strcasecmp(const char *f1, const char *f2);

char *My_strchr(const char *str, int c);

char *My_strdup(const char *str);

char *My_strcat(char *destination, const char *source);

char *My_strncat(char *destination, const char *source, size_t num);

char *My_strstr(const char *haystack, const char *needle);

size_t My_strspn(const char *str1, const char *str2);

char *My_strtok(char *str, const char *delim);
