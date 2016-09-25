#ifndef _MYIO_H_
#define _MYIO_H_

#include <stdio.h>
#include <stdarg.h>

char * mygets(char * str);
void myputs(const char * str);
void myprintf(const char * format, ...);
void myscanf(const char * format, ...);

#endif
