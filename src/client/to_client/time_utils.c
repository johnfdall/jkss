#include <time.h>
#include <stdio.h>

void print_time() {
        time_t timer;
        time(&timer);
        struct tm *timeinfo;
        timeinfo = gmtime(&timer);
        char date[128];
        strftime(date, 128, "%a, %d %b %Y %H:%M:%S +0000", timeinfo);
        printf("%s\n", date);
}
