#include "error.h"

char* Error_sev_to_cstr(Error_Severity sev){
    switch(sev){
        case ERROR_SEVERITY_INFO: return "\x1b[36mINFO: \x1b[0m";
        case ERROR_SEVERITY_WARNING: return "\x1b[35mWARNING: \x1b[0m";
        case ERROR_SEVERITY_FATAL: return "\x1b[31mFATAL: \x1b[0m";
    }
    return NULL;
}