#pragma once
#include <stdint.h>

typedef struct{
    char* data;
    unsigned long long count;
} String;

String string_from_cstr(char* cstr);
String string_slurp_file(const char* file_path);
String string_chopByDelim(String *str, char delim);
String string_trim_left(String str);
String string_trim_right(String str);
String string_trim(String str);
int string_eq(String str1, String str2);
uint64_t string_to_int(String str);