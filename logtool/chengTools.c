#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "chengTools.h"

unsigned long get_file_size(const char *filename)  
{  
    struct stat buf;  
    if(stat(filename, &buf)<0)  
    {  
    	printf("get_file_size error\n");
        return 0;  
    }  
    return (unsigned long)buf.st_size;  
}


int readini(char *filename,char *PName,char *RName,char *result)
{
	FILE * fp;
	char buf[256],PName1[32],RName1[32];
	int s,i;
	char *pp;
	if ((fp = fopen(filename,"rt")) == NULL)
		return 0;
	sprintf(PName1,"[%s]",PName);
	sprintf(RName1,"%s=",RName);
	
	memset(buf,0,sizeof(buf));
	s=0;
	while(fgets(buf, sizeof(buf), fp))
	{
//		printf("%s",buf);
		if(strstr(buf,PName1)!=NULL)
		{	s=1;	continue;	}
		if(s==0)
			continue;
		else
		{
			if(strstr(buf,RName1)!=NULL)
			{
				strcpy(result,buf+strlen(RName1));
				for(i=0;i<strlen(result);i++)
				{
					if(result[i]==0x0d||result[i]==0x0a)
					{	result[i]=0;	break;	}
				}
				fclose(fp);
				return 1;
			}
		}
		if((strstr(buf,"[")!=NULL)&&(strstr(buf,"]")!=NULL))
		{
			s=0;
		}
		memset(buf,0,sizeof(buf));
	}
	fclose(fp);
	return 0;
}


/*
struct tm   
{   
int tm_sec;//seconds 0-61    
int tm_min;//minutes 1-59    
int tm_hour;//hours 0-23    
int tm_mday;//day of the month 1-31    
int tm_mon;//months since jan 0-11    
int tm_year;//years from 1900    
int tm_wday;//days since Sunday, 0-6    
int tm_yday;//days since Jan 1, 0-365    
int tm_isdst;//Daylight Saving time indicator    
};  
*/

//return seconds from 1970 ,return YYYY-MM-DD hh:mm:ss using strTime
time_t getSystemTime( char* strTime){
	time_t now;
	struct tm *timeNow;
	//char fmtTime[20];
	
	
	time(&now);
	timeNow = localtime(&now);
	//memset(fmtTime,0,sizeof(fmtTime));
	sprintf(strTime,"%d-%02d-%02d %02d:%02d:%02d",timeNow->tm_year+1900,timeNow->tm_mon+1,timeNow->tm_mday,timeNow->tm_hour,timeNow->tm_min,timeNow->tm_sec);
	//strcpy(strTime)
	return now;
}

/*
功能：将YYYY-MM-DD mm:hh:ss 转换成struct tm
参数：chineseDatetime :YYYY-MM-DD mm:hh:ss 格式字符串；tm struct tm结构体，返回使用
返回：
*/
int datetime_to_tm(char *chineseDatetime,struct tm *tm){
	char *p = NULL;
	char t1[6][5];
	int i=0,j=0;
	p = chineseDatetime;

	memset(t1,0,sizeof(t1));
	
	while(*(p) != ' '){

		if(*p == '-'){
			j++;
			i=0;
			p++;
		}
		t1[j][i++] = *p;
		p++;

	}

	j++;
	i=0;

	while(*(p) != '\0'){
		if(*p == ':'){
			j++;
			i=0;
			p++;
		}
		t1[j][i++] = *p;
		p++;
	}
	
	tm->tm_year = atoi(t1[0])-1900;
	tm->tm_mon = atoi(t1[1])-1;
	tm->tm_mday = atoi(t1[2]);
	tm->tm_hour = atoi(t1[3]);
	tm->tm_min = atoi(t1[4]);
	tm->tm_sec = atoi(t1[5]);
	
	return 0;
}

int tm_to_datetime(struct tm *tm,char *chineseDatetime){
	char cDT[20];
	
	memset(cDT,0,sizeof(cDT));

	sprintf(cDT,"%04d-%02d-%02d %02d:%02d:%02d\0",tm->tm_year + 1900,tm->tm_mon + 1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);

	strcpy(chineseDatetime,cDT);
	return 0;
}

long diff_datetime(char *dt1,char *dt2){
	struct tm tm1,tm2;
	time_t t1,t2;
	memset(&tm1,0,sizeof(tm1));
	memset(&tm2,0,sizeof(tm2));
	datetime_to_tm(dt1,&tm1);
	datetime_to_tm(dt2,&tm2);
	
	t1 = mktime(&tm1);
	t2 = mktime(&tm2);
	//printf("diff_datetime:dt1=%s\tdt2=%s\tt1=%d\tt2=%d\n",dt1,dt2,t1,t2);
	return (long)(t2 - t1);
}
int timestamp_to_datetime(char *tmstp,char *dt){
	unsigned int iPos = 0;
	iPos = (unsigned int)strchr(tmstp,'.');
	printf("timestmp is %s\n.pos is :%d\n",tmstp,iPos);
	if(iPos == 0){
		strcpy(dt,tmstp);
		return 0;
	}
	if(iPos > 0){
		strncpy(dt,tmstp,iPos - (unsigned int)tmstp);
		return 1;
	}
}
/*
功能：读取ini文件
参数：filename ini文件名称；PName 节名称；RName 项名称 ；result 返回值
返回：成功 1；失败 0
作者：郭磊
时间：
*/
int cbt_readini(char *filename,char *PName,char *RName,char *result)
{
	FILE * fp;
	char buf[256],PName1[32],RName1[32],cRet[1024];
	int s,i;
	char *pp;
	if ((fp = fopen(filename,"rt")) == NULL)
		return 0;
	sprintf(PName1,"[%s]",PName);
	sprintf(RName1,"%s=",RName);
	
	memset(buf,0,sizeof(buf));
	s=0;
	while(fgets(buf, sizeof(buf), fp))
	{
		//printf("%s",buf);
		if(strstr(buf,PName1)!=NULL)
		{	s=1;	continue;	}
		if(s==0)
			continue;
		else
		{
			if(strstr(buf,RName1)!=NULL)
			{
				strcpy(cRet,buf+strlen(RName1));
//				printf("rname=%s\n",RName1);
//				printf("result1=%s\n",cRet);
				for(i=0;i<strlen(cRet);i++)
				{
					if(cRet[i]==0x0d||cRet[i]==0x0a)
					{	cRet[i]=0;	break;	}
				}
			//	printf("result2=%s\n",result);
				strcpy(result,cRet);
				return 1;
			}
		}
		if((strstr(buf,"[")!=NULL)&&(strstr(buf,"]")!=NULL))
		{
			s=0;
		}
		memset(buf,0,sizeof(buf));
	}
	return 0;
}
