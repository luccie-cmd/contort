#pragma once
#include <stdarg.h>
#include <stdio.h>


typedef enum{
    ERROR_SEVERITY_INFO,
    ERROR_SEVERITY_WARNING,
    ERROR_SEVERITY_FATAL,
} Error_Severity;

char* Error_sev_to_cstr(Error_Severity sev);
#define ERROR(sev, msg, ...) {printf("%s", Error_sev_to_cstr(sev)); printf(msg, __VA_ARGS__);}