#ifndef CHENGTOOLS
#define CHENGTOOLS
#include <time.h>
#include <stdio.h>
unsigned long get_file_size(const char *filename) ;
int readini(char *filename,char *PName,char *RName,char *result);
time_t getSystemTime( char* strTime);

#define DAY_SECOND 86400
//��YYYY-MM-DD mm:hh:ss ת����struct tm
int datetime_to_tm(char *chineseDatetime,struct tm *tm);
//��struct tm ת����YYYY-MM-DD mm:hh:ss
int tm_to_datetime(struct tm *tm,char *chineseDatetime);

//��������YYYY-MM-DD mm:hh:ss �����������db1 �� dt2 ��Ҫ������
long diff_datetime(char *dt1,char *db2);



//ʱ���תdatetime,�ɹ�����1��δ����΢�벿�֣���ȫ�����ƣ�����0
int timestamp_to_datetime(char *tmstp,char *dt);
/*
���ܣ���ȡini�ļ�
������filename ini�ļ����ƣ�PName �����ƣ�RName ������ ��result ����ֵ
���أ��ɹ� 1��ʧ�� 0
���ߣ�����
ʱ�䣺
*/
int cbt_readini(char *filename,char *PName,char *RName,char *result);

char *cheng_fgets(char *s, int n,  FILE *stream);
#endif