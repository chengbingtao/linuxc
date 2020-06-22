#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "chengTools.h"
#include "biTree.h"
#include "ftpTool.h"
#include "zcSend.h"
#include "list.h"
#include "outFile.h"
#define MAX_LINE 2048
#define PROFILE "profile.ini"

typedef struct _logData{
	char lsh[20];
	char cpkey[22];
	} StrLogData;



void destroyPiao(void* data){
	struct piao *piao = (struct piao*)data;
	//printf("destroyPiao:%s\n",piao->cpkey);
	free(piao);
	piao=NULL;
}

void destroyLog(void* data){
	StrLogData *p = (StrLogData*)data;
	//printf("destroyPiao:%s\n",piao->cpkey);
	free(p);
	p=NULL;
}

//struct biTreeNode **succTree=NULL;

//localPath	:the folder to store output file that download from ftp server
//ftpPath	:the folder that output file store any day
//function	:get outputfile from ftp server
//return	:if success the number of transed files;if no file trans,return 0;error -n 
int getOutFiles(char* ftpPath,char* localPath)
{
	
return 0;
}

/*
 *function	:delete the output files used over,don't delete the folder
 *localPath	:the folder that output file stored
 *return	:if success the number of deleted files,if no file be deleted,return 0,if error return -n
 * */
int deleteFiles(char* localPath)
{
return 0;
}


/*
 *function	:trieve log file,get records that status ar 4020,4030 and send successful 
 *successTree	:search tree data struct,store success record in log files
 *logFileArray	:all log files name
 *return	:the node number of successTree,means records 
 * */


unsigned long getLogRecords( List *list4020,List *list4030,char* logFileName,char* retFile)
{
	//char* fileName;
	FILE* fp=NULL;
	int i;
	char cLine[MAX_LINE];
	char *p1=NULL;
	char *p2=NULL;
	char cLsh[20];
//	char fileNameNew[1024];
//	FILE* fp4020,*fp4030;
	FILE* fpRet;
	unsigned long ul_1=0;
	//unsigned long ul_2=0;
	StrLogData *pLogData=NULL;
	
	//sprintf(fileNameNew,"%s.return",fileName);
	fpRet = fopen(retFile,"w");
	
	//for(i=0;i<fileCount;i++){
		//fileName = logFileArray;
		fp = fopen(logFileName,"r");
		if(fp == NULL) return -1;
		if(fpRet == NULL) return -2;	
			
		memset(cLine,0,sizeof(cLine));
		while(fgets(cLine,LINE_MAX,fp) != NULL){
			//ul_2++;
			//printf("begin:%d\t",ul_2);
			p1 = strstr(cLine,"content=");
			if(p1){
				
				//p1 += 8;	//strlen("content=")
				if(strncmp(p1 + 8,"4020",4) ==0){
					pLogData = calloc(1,sizeof(StrLogData));
					//memset(cLsh,0,sizeof(cLsh));
					strncpy(pLogData->lsh,p1 + 13,18);
				//	fputs(cLsh,fp4020);
					//insertNode(bitree4020,cLsh);
					
					p2 = strstr(p1+13,":")+1;
					if(p2) strncpy(pLogData->cpkey,p2,17);
					
					list_insert_next(list4020,list_tail(list4020), pLogData);	
					
					//pLogData = (StrLogData*)list_tail(list4020)->data;
					//printf("insert lsh=%s\tcpkey=%s\n",pLogData->lsh,pLogData->cpkey);
					ul_1++;
				}

				if(strncmp(p1 + 8,"4030",4) ==0){
					//memset(cLsh,0,sizeof(cLsh));
					pLogData = calloc(1,sizeof(StrLogData));
					strncpy(pLogData->lsh,p1 + 13,18);
					
					p2 = strstr(p1+13,":")+1;
					if(p2) strncpy(pLogData->cpkey,p2,17);
					
					list_insert_next(list4030,list_tail(list4030), pLogData);	
				//	fputs(cLsh,fp4030);
					//insertNode(bitree4020,cLsh);
					ul_1++;
				}
			}
			
			//printf("v1\t");

			p1 = strstr(cLine,"pack=@");
			if(p1){
				p1 = strstr(cLine,"|0$");
				if(p1){
					memset(cLsh,0,sizeof(cLsh));
					strncpy(cLsh,p1+3,18);
				//	strcat(cLsh,"\n");
					fputs(cLsh,fpRet);
					ul_1++;
				}
			}
			memset(cLine,0,sizeof(cLine));	
			//printf("v2\n");
		}
		fclose(fp);	
		
	//}
	fclose(fpRet);
	
	//给链表加一个无含义的head,因为链表只有删除下级节点的功能，这样的话，从空头出发，可以删除当前节点
	pLogData = calloc(1,sizeof(StrLogData));
	list_insert_next(list4020,NULL,pLogData);
	pLogData = calloc(1,sizeof(StrLogData));
	list_insert_next(list4030,NULL,pLogData);
	
	return ul_1;
}

/*
compare list and srcFileName,get records which sended ok,write to tgtFileName
*/
void compareLogRecords(List* pList4020,List* pList4030,char* srcFileName,char* tgtFileName){
	FILE* fpRet=NULL;
	FILE* fpTgt=NULL;
	//char fileNameNew[1024];
	char cLine[20];
	unsigned long ul_1=0;
	ListElement *ple=NULL;
	ListElement *pleNext=NULL;
	StrLogData *plsd=NULL;
	int iFound=0;
	//sprintf(fileNameNew,"%s.return",fileName);
	fpRet = fopen(srcFileName,"r");
	fpTgt = fopen(tgtFileName,"a");
	memset(cLine,0,sizeof(cLine));
	while(fread(cLine,18,1,fpRet) != NULL){
		iFound=0;
		//printf("168 cLine:%s\n",cLine);
		ple=list_head(pList4030);
		while(list_next(ple)!=NULL){
			pleNext = list_next(ple);
			plsd = (StrLogData*)pleNext->data;
			
			if(strcmp(plsd->lsh,cLine) == 0){
				plsd=NULL;
				list_remove_next(pList4030,ple,(void**)&plsd);
				//printf("178 cpk:%s\n",plsd->cpkey);
				fputs(plsd->cpkey,fpTgt);
				free(plsd);
				
				
				iFound = 1;
				break;
			}
			ple = list_next(ple);
			ul_1++;
		}
		
		if(iFound == 1) continue;
			
		ple=list_head(pList4020);
		while(list_next(ple)!=NULL){
			pleNext = list_next(ple);
			plsd = (StrLogData*)pleNext->data;
			//printf("listelement lsh=%s\tcpkey=%s\n",plsd->lsh,plsd->cpkey);
			if(strcmp(plsd->lsh,cLine) == 0){
				plsd=NULL;
				list_remove_next(pList4020,ple,(void**)&plsd);
				fputs(plsd->cpkey,fpTgt);
				//printf("193cpkey:%s\n",plsd->cpkey);
				free(plsd);
				
				
				//iFound = 1;
				break;
			}
			ple = list_next(ple);
			ul_1++;
		}	
		
		
		
		
		memset(cLine,0,sizeof(cLine));
	}

	fclose(fpRet);
	fclose(fpTgt);
	//printf("function compareTree : %d rows read to delete tree\n",ul_1);
}

unsigned long PutKeyToTree(BiTree* pBiTree,char* keyFile)
{
	FILE* fpKey=NULL;
	char cLine[18];
	unsigned long ul_1=0;
	fpKey = fopen(keyFile,"r");
	memset(cLine,0,sizeof(cLine));
	while(fread(cLine,17,1,fpKey) != NULL){
		insertNode(pBiTree, cLine);
		memset(cLine,0,sizeof(cLine));
		ul_1++;
	}
	//
	fclose(fpKey);
	return ul_1;
}

//every line of file:23010101|10001|... the last line is md5
long putXszbmZdhToArray(long* pArray,char* fileName){
	FILE* fp=NULL;
	char cLine[1024];
	char cXszbm[10];
	char cZdh[10];
	char *p=NULL,*p2=NULL;
	long l_1=0,lXszbm,lZdh;
	fp = fopen(fileName,"r");
	memset(cLine,0,sizeof(cLine));
	while(fgets(cLine,sizeof(cLine),fp)){
		l_1++;
		p = strchr(cLine,'|');
		if(p==NULL) break;
		
		memset(cXszbm,0,sizeof(cXszbm));
		strncpy(cXszbm,cLine,(unsigned int)p - (unsigned int)cLine	);
		
		p++;
		p2 = strchr(p,'|');
		memset(cZdh,0,sizeof(cZdh));
		strncpy(cZdh,p,(unsigned int)p2 - (unsigned int)p );
		
		lXszbm = atol(cXszbm);
		lZdh = atol(cZdh);
		//printf("zdh=%d,xszbm=%d\t",lZdh,lXszbm);
		pArray[lZdh] = lXszbm;
		
		
		
		memset(cLine,0,sizeof(cLine));
		//fgets(cLine,sizeof(cLine),fp);
	}
	fclose(fp);
	return l_1;
}

//get output file's names from name list file,and then put them to string array,return num of file's names

int putOutputNameToArray(char pName[][1024],char* fileName){
	FILE* fp=NULL;
	char cLine[1024];
	int i_1=0,i3;
	char *p=NULL;
	
	fp = fopen(fileName,"r");
	memset(cLine,0,sizeof(cLine));
	while(fgets(cLine,sizeof(cLine),fp)){
		
		
		
		//printf("\ncLine=%s\n",cLine);
		//if(strncmp(cLine,"***",3) != 0){
			i3=0;
			//不用strcpy的原因，cLine读取一行，会把\r\n也读进来,strcpy会复制这些无效字符到文件名中
			for(p=(char*)cLine;!((char)*p=='\r' || (char)*p == '\n' ||(char)*p =='\0');p++)
				pName[i_1][i3++] = (char)*p;
			//strcpy(pName[i_1],cLine);
			i_1++;
		//}
		
		
		
		memset(cLine,0,sizeof(cLine));
	}
	fclose(fp);
	return i_1;
} 

//get dates  from name list file,and then put them to date array,return num of dates
//in name list file,the last line like :***2010-01-01***2010-01-02***2010-01-03 
int putDatesToArray(char pName[][11],char* fileName){
	FILE* fp=NULL;
	char cLine[1024];
	char* p=NULL;
	int i_1=0,i3;
	//char cDate[50][11];
	fp = fopen(fileName,"r");
	//printf("329 filename=%s\tfp=%d\n",fileName,fp);
	memset(cLine,0,sizeof(cLine));
	while(fgets(cLine,sizeof(cLine),fp)){
		i3=0;
		//printf("cLine:%s\n",cLine);
			//不用strcpy的原因，cLine读取一行，会把\r\n也读进来,strcpy会复制这些无效字符到文件名中
			for(p=(char*)cLine;!((char)*p=='\r' || (char)*p == '\n' ||(char)*p =='\0');p++)
				pName[i_1][i3++] = (char)*p;
			//strncpy(pName[i_1],cLine,10);
			//printf("pName:%s\n",pName[i_1]);
			i_1++;
		
		
		memset(cLine,0,sizeof(cLine));
	}
	fclose(fp);
	
	/*
	p = cLine;
	if(p=strstr(p,"***")) p+=3;
	while(p){
		
		strncpy(pName[i_1],p,10);
		printf("341 %d:%s\n",i_1,pName[i_1]);
		i_1++;
		if(p=strstr(p,"***")) p+=3;
		
	}
	*/
	return i_1;
} 



int main(int argc, char* argv[]){
	
	int iRet=0;
	int i,iDateNum,i2;
	int iRet2=0;
	char c[10];
	//List listOut;	
	ListElement *ple=NULL;
	long lRet;
	struct piao *pTicket=NULL;
	char fmtNow[20];
	char fmtDate[11];
	char jvwgPath[1024];
	char retPath[1024];
	char jvwgFile[1024];
	char retFile[1024];
	char keyFile[1024];
	char jvwgFileArray[100][1024];
	char outFileArray[100][1024];
	char dateArray[100][11];
	unsigned long recNum;
	List list4020,list4030;
	StrLogData *psld=NULL;
	
	BiTree biTreeKey=NULL;
	BiTreeNode *pTreeNode=NULL;
	char ftpPath[1024];
	char ftpPath2[1024];
	char localPath[1024];
	char localPath2[1024];
	char ftpUser[128];
	char ftpPwd[128];	
	time_t tNow,tYestoday;
	struct tm *ptmTmp;
	char cFmtYestodayTzz[20],cFmtToday[20];
	long xszbmzdh[40000];
	char jvwgIp[20];
	unsigned int jvwgPort=0;
	
	//download yestoday tzz file
	memset(ftpPath,0,sizeof(ftpPath));
	readini(PROFILE,"tzz","ftp_path",ftpPath);
	if(strlen(ftpPath)==0){
		printf("tzz ftpPath read from ini error:%s\n",PROFILE);
		return -1;
	}
	memset(localPath,0,sizeof(localPath));
	readini(PROFILE,"tzz","local_path",localPath);
	if(strlen(localPath)==0){
		printf("tzz localPath read from ini error:%s\n",PROFILE);
		return -2;
	}
	memset(ftpUser,0,sizeof(ftpUser));
	readini(PROFILE,"tzz","ftp_user",ftpUser);
	if(strlen(ftpUser)==0){
		printf("tzz ftp_user read from ini error:%s\n",PROFILE);
		return -3;
	}
	memset(ftpPwd,0,sizeof(ftpPwd));
	readini(PROFILE,"tzz","ftp_pwd",ftpPwd);
	if(strlen(ftpPwd)==0){
		printf("tzz ftp_pwd read from ini error:%s\n",PROFILE);
		return -4;
	}
	
	time(&tNow);
	tNow -= DAY_SECOND;
	ptmTmp = localtime(&tNow);
	memset(cFmtYestodayTzz,0,sizeof(cFmtYestodayTzz));
	//tm_to_datetime(ptmTmp,cFmtYestoday);
	sprintf(cFmtYestodayTzz,"dn%04d%02d%02d.tzz",ptmTmp->tm_year + 1900 ,ptmTmp->tm_mon + 1, ptmTmp->tm_mday);
	
	sprintf(ftpPath,"%s%s",ftpPath,cFmtYestodayTzz);
	//sprintf(localPath,"%s%s",localPath,cFmtYestodayTzz);
	//每次覆盖同名文件，如果下载不成功，则沿用之前生成的，仅提示错误
	sprintf(localPath,"%ss_tzz.txt",localPath);
	iRet = downloadsimple(ftpUser,ftpPwd,ftpPath,localPath);
	
	//printf("read tzz file begin!\n");
	
	//填充xszbmzdh数组
	if(get_file_size(localPath) >0){
		memset(xszbmzdh,0,sizeof(xszbmzdh));
		lRet = putXszbmZdhToArray(xszbmzdh,localPath);
		printf("================read tzz file over!line %d================\n",lRet);
				for(iRet =lRet;iRet>=0;iRet--){
					//printf("<%d,%d>\t",iRet,xszbmzdh[iRet]);
				}
				printf("\n");
	}else{
		printf("================read tzz file error!================\n");
	}
	
	
	
	//download out file
	memset(ftpPath,0,sizeof(ftpPath));
	readini(PROFILE,"output","ftp_path",ftpPath);
	if(strlen(ftpPath)==0){
		printf("output ftpPath read from ini error:%s\n",PROFILE);
		return -5;
	}
	memset(localPath,0,sizeof(localPath));
	readini(PROFILE,"output","local_path",localPath);
	if(strlen(localPath)==0){
		printf("output localPath read from ini error:%s\n",PROFILE);
		return -6;
	}
	memset(ftpUser,0,sizeof(ftpUser));
	readini(PROFILE,"output","ftp_user",ftpUser);
	if(strlen(ftpUser)==0){
		printf("output ftp_user read from ini error:%s\n",PROFILE);
		return -7;
	}
	memset(ftpPwd,0,sizeof(ftpPwd));
	readini(PROFILE,"output","ftp_pwd",ftpPwd);
	if(strlen(ftpPwd)==0){
		printf("output ftp_pwd read from ini error:%s\n",PROFILE);
		return -8;
	}
	
	//download namelist file
	time(&tNow);
	ptmTmp = localtime(&tNow);
	memset(cFmtToday,0,sizeof(cFmtToday));
	//tm_to_datetime(ptmTmp,cFmtYestoday);
	sprintf(cFmtToday,"%04d%02d%02dout.log",ptmTmp->tm_year + 1900 ,ptmTmp->tm_mon + 1, ptmTmp->tm_mday);
	sprintf(ftpPath2,"%s%s",ftpPath,cFmtToday);
	
	sprintf(localPath2,"%s%s",localPath,cFmtToday);
	downloadsimple(ftpUser,ftpPwd,ftpPath2,localPath2);
	
	printf("================download output file name:%s over!================\n",localPath2);
	
	memset(outFileArray,0,sizeof(outFileArray));
	//printf("outFile path=%s\n",localPath2);
	iRet = putOutputNameToArray(outFileArray,localPath2);
	if(iRet==0){
		printf("there is not output's names in file %s\n",localPath2);
		return -9;
	}
	printf("================read output file name over!================\n" );
	
	
	
	
	
	memset(dateArray,0,sizeof(dateArray));
	sprintf(cFmtToday,"%04d%02d%02ddate.log",ptmTmp->tm_year + 1900 ,ptmTmp->tm_mon + 1, ptmTmp->tm_mday);
	sprintf(ftpPath2,"%s%s",ftpPath,cFmtToday);
	sprintf(localPath2,"%s%s",localPath,cFmtToday);
	downloadsimple(ftpUser,ftpPwd,ftpPath2,localPath2);
	iDateNum = putDatesToArray(dateArray,localPath2);
	printf("================get %d Dates of Qi over!================\n",iDateNum);
	if(iDateNum ==0){
		printf("there is not date array in file %s\n",localPath);
		return -10;
	}
	for(i=0;i<iDateNum;i++){
		printf("date of Qi %d:%s\n",i,dateArray[i]);
	}
	
	
	//download output file
	printf("================download output file begin!================\n");
	for(i=0;i<iRet;i++){
		sprintf(ftpPath2,"%s%s",ftpPath,outFileArray[i]);
		sprintf(localPath2,"%s%s",localPath,outFileArray[i]);
		//printf("ftp-Path:%s\tlocal-path:%s\tftp-user:%s\tftp-pwd:%s\n",ftpPath2,localPath2,ftpUser,ftpPwd);
		downloadsimple(ftpUser,ftpPwd,ftpPath2,localPath2);
		memset(outFileArray[i],0,1024);
		strcpy(outFileArray[i],localPath2);
		printf("down load output file:%s\n",outFileArray[i]);
	}
	printf("================download output file over!================\n");
	
	
	//ftp test ok!
	//char* ftp_path="ftp://192.168.1.108/Documents/t1.txt";
	//char* local_path="./t1.txt";
	//char* user="hdper";
	//char* pwd="hdper";

	//downloadsimple(user,pwd,ftp_path,local_path);
	


	//socket client test ok!
	//iRet = init_socket("192.168.31.88",9527);
	//sendbuff(iRet,"1234567890",10);
	//memset(c,0,10);
	//printf("sendbuff over\n");
	//iRet2 = recvbuff(iRet,c,&iRet);
	//if(0>iRet2) printf("recvbuff return %d\n,",iRet2); 
	//printf("return len:%d ;%s\n",iRet,c);
	//close(iRet);
	
	//store out file to link successful
	/*
	list_init(&listOut,destroyPiao);
	lRet = getTicket("./outfile/20200531/2019001lotls0000.out",&listOut);
	lRet = getTicket("./outfile/20200531/2019001lotfs0000.out",&listOut);
	
	ple = list_head(&listOut);

	pTicket = (struct piao *)list_data(ple);
	printf("%s;tiaoshu=%d;zhushu=%d\n",pTicket->cpkey,pTicket->tiaoshu,pTicket->tzs);
	while(true){
		//pTicket = (struct piao *)list_data(ple);
		//printf("%s\n",pTicket->cpkey);
		ple=list_next(ple);
		pTicket = (struct piao *)list_data(ple);
		printf("%s;tiaoshu=%d;zhushu=%d\n",pTicket->cpkey,pTicket->tiaoshu,pTicket->tzs);
		if(list_is_tail(ple))break;
	}
	list_destroy(&listOut);
	*/
	
	memset(fmtNow,0,sizeof(fmtNow));
	getSystemTime(fmtNow);
	//printf("=================%s=================\n",fmtNow);
	//sprintf(fmtDate,"2020-05-22");
	memset(fmtDate,0,sizeof(fmtDate));
	strncpy(fmtDate,fmtNow,10);
	memset(jvwgPath,0,sizeof(jvwgPath));
	readini("profile.ini","jvwgfile","path",jvwgPath);
	
	memset(retPath,0,sizeof(retPath));
	readini("profile.ini","jvwgfile","return_path",retPath);
	//memset(c,0,sizeof(c));
	//strncpy(c,fmtDate,4);
	sprintf(retFile,"%s%s.txt",retPath,fmtDate);
	sprintf(keyFile,"%s%s.key",retPath,fmtDate);
	
	//printf("retFile=%s	keyFile=%s\n",retFile,keyFile);
	//	goto gt636;	
	//delete keyFile
	if(0==access(keyFile,F_OK)) remove(keyFile);
	
	//delete retFile
	if(0==access(retFile,F_OK)) remove(retFile);	
		

	//i=0;
	list_init(&list4020,destroyLog);
	list_init(&list4030,destroyLog);
	printf("================treate log file begin!================\n");
	//按本期所有日期，遍历所有日志文件
	for(i2=0;i2<iDateNum;i2++){
		i=0;
		while(true){
			sprintf(jvwgFile,"%sdebug-%s.%d.log",jvwgPath,dateArray[i2],i);
			if(-1==access(jvwgFile,F_OK)) break;
			
			
			recNum = getLogRecords( &list4020,&list4030,jvwgFile,retFile);
			printf("getLogRecords:%s over!\n",jvwgFile);
			
	
			printf("4020 len=%d\t4030 len=%d\n",list_size(&list4020),list_size(&list4030));
			//break;
			
			
			
			compareLogRecords(&list4020,&list4030,retFile,keyFile);
			printf("compareLogRecords:%s over!\n",jvwgFile);
			printf("4020 len=%d\t4030 len=%d\n",list_size(&list4020),list_size(&list4030));
			
			i++;
			
		
		}
	}
	list_destroy(&list4020);
	list_destroy(&list4030);
	//printf("after 4020 len=%d\t4030 len=%d\n",list_size(&list4020),list_size(&list4030));
	printf("================treate log file over!================\n");
	
	gt636:
	
	//put key file to tree
	recNum = PutKeyToTree(&biTreeKey,keyFile);
	printf("count = %d\tmax key = %s\tmin key = %s\n",recNum,maxImum(&biTreeKey)->data,minImum(&biTreeKey)->data);
	
	
	
	
	
	
	memset(jvwgIp,0,sizeof(jvwgIp));
	memset(c,0,sizeof(c));
	readini(PROFILE,"jvwgfile","ip",jvwgIp);
	readini(PROFILE,"jvwgfile","port",c);
	jvwgPort = atoi(c);
	printf("java netgate ip:%s\tport:%d\n",jvwgIp,jvwgPort);
	for(i=0;i<iRet;i++){
		if(-1==access(outFileArray[i],F_OK)) break;
		printf("begin compareTicket:%s\n",outFileArray[i]);
		
		if(0 < compareTicket(outFileArray[i],&biTreeKey)){
			printf("main 659\n");
			sprintf(jvwgFile,"%s_N",outFileArray[i]);
			iRet2 = sendXkjRz(jvwgFile,jvwgIp,jvwgPort,xszbmzdh);
			if(iRet2 <= 0){
				printf("send jvwg error :file=%s\treturn=%d\n",jvwgFile,iRet2);
			}
		}
		
		 
	}
	
	//destroy key tree
	while(pTreeNode = minImum(&biTreeKey)){
		deleteNode2(&biTreeKey, pTreeNode); 
	}
	
	
	
	exit(0);


}
