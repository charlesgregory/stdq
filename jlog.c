#include <stdio.h>
#include <stdarg.h>
#include <time.h>

void jlog(char *format, ...)
{
    time_t t = time(NULL);
    struct tm * timedata = localtime(&t);
    char timebuf[32];
    va_list aptr;
    
    strftime(&timebuf[0], sizeof(timebuf), "%Y-%m-%dT%T", timedata);

    fprintf(stderr, "[stdpub %s]\t", timebuf);
    
    va_start(aptr, format);
    vfprintf(stderr, format, aptr);
    va_end(aptr);

    fprintf(stderr, "\n");    
}
