#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include "logger.h"

//Font color
#define Black 30
#define Blue 34
#define Yellow 33
#define Red 31
#define Magenta 35

int isSyslog = 0;

int initLogger(char *logType) {
    if(strcmp(logType,"syslog")==0){
        isSyslog = 1;
    }
    printf("Initializing Logger on: %s\n", logType);
    return 0;
}

int infof(const char *format, ...){
    //prints the text in the selected color
    printf("\033[1;34m");
    va_list args;
    int done;
    va_start(args, format);
    if(isSyslog==1){
        done = 0;
        vsyslog(LOG_INFO,format,args);
        closelog();
    }else{
        done = vprintf(format, args);
    }
    va_end(args);
    //Resets the color
    printf("\033[0m");
    return done;
}

int warnf(const char *format, ...) {
    //prints the text in the selected color
    printf("\033[1;33m");
    va_list args;
    int done;
    va_start(args, format);
    if(isSyslog==1){
        done = 0;
        vsyslog(LOG_WARNING,format,args);
        closelog();
    }else{
        done = vprintf(format, args);
    }
    va_end(args);
    //Resets the color
    printf("\033[0m");
    return done;
    
}

int errorf(const char *format, ...) {
    //prints the text in the selected color
    printf("\033[1;31m");
    va_list args;
    int done;
    va_start(args, format);
        if(isSyslog==1){
        done = 0;
        vsyslog(LOG_ERR,format,args);
        closelog();
    }else{
        done = vprintf(format, args);
    }
    va_end(args);
    //Resets the color
    printf("\033[0m");
    return done;
}

int panicf(const char *format, ...) {
    //prints the text in the selected color
    printf("\033[1;35m");
    va_list args;
    int done;
    va_start(args, format);
        if(isSyslog==1){
        done = 0;
        vsyslog(LOG_EMERG,format,args);
        closelog();
    }else{
        done = vprintf(format, args);
    }
    va_end(args);
    //Resets the color
    printf("\033[0m");
    return done;
}
