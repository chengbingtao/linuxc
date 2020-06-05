#ifndef CHENGTOOLS
#define CHENGTOOLS
#include <time.h>
unsigned long get_file_size(const char *filename) ;
int readini(char *filename,char *PName,char *RName,char *result);
time_t getSystemTime( char* strTime);
#endif