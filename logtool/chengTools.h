#ifndef CHENGTOOLS
#define CHENGTOOLS
#include <time.h>
#include <stdio.h>
unsigned long get_file_size(const char *filename) ;
int readini(char *filename,char *PName,char *RName,char *result);
time_t getSystemTime( char* strTime);

#define DAY_SECOND 86400
//将YYYY-MM-DD mm:hh:ss 转换成struct tm
int datetime_to_tm(char *chineseDatetime,struct tm *tm);
//将struct tm 转换成YYYY-MM-DD mm:hh:ss
int tm_to_datetime(struct tm *tm,char *chineseDatetime);

//计算两个YYYY-MM-DD mm:hh:ss 的秒数差，即从db1 到 dt2 需要多少秒
long diff_datetime(char *dt1,char *db2);



//时间戳转datetime,成功返回1，未发现微秒部分，则全部复制，返回0
int timestamp_to_datetime(char *tmstp,char *dt);
/*
功能：读取ini文件
参数：filename ini文件名称；PName 节名称；RName 项名称 ；result 返回值
返回：成功 1；失败 0
作者：郭磊
时间：
*/
int cbt_readini(char *filename,char *PName,char *RName,char *result);

char *cheng_fgets(char *s, int n,  FILE *stream);
#endif