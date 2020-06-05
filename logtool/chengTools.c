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