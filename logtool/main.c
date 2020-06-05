#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
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




int main(int argc, char* argv[]){
	
	int iRet=0;
	int i;
	int iRet2=0;
	char c[10];
	List listOut;	
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
	unsigned long recNum;
	List list4020,list4030;
	StrLogData *psld=NULL;
	//BiTree biTree4020;
	//BiTree biTree4030;
	//BiTreeNode* btrnTmp;
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
	printf("=================%s=================\n",fmtNow);
	sprintf(fmtDate,"2020-05-22");
	memset(jvwgPath,0,sizeof(jvwgPath));
	readini("profile.ini","jvwgfile","path",jvwgPath);
	
	memset(retPath,0,sizeof(retPath));
	readini("profile.ini","jvwgfile","return_path",retPath);
	//memset(c,0,sizeof(c));
	//strncpy(c,fmtDate,4);
	sprintf(retFile,"%s%s.txt",retPath,fmtDate);
	sprintf(keyFile,"%s%s.key",retPath,fmtDate);
	
	//printf("retFile=%s	keyFile=%s\n",retFile,keyFile);
	
	
	i=0;
	list_init(&list4020,destroyLog);
	list_init(&list4030,destroyLog);
	while(true){
		sprintf(jvwgFile,"%sdebug-%s.%d.log",jvwgPath,fmtDate,i);
		//printf("%s treated begin!\n",jvwgFile);	
		if(-1==access(jvwgFile,F_OK)) break;
		printf("%s treated begin!\n",jvwgFile);	
		
		//strcpy(jvwgFileArray[i] ,jvwgFile); 
		
		recNum = getLogRecords( &list4020,&list4030,jvwgFile,retFile);
		printf("getLogRecords:%s over!\n",jvwgFile);
		
//		ple = list_head(&list4020);
//		while(ple){
//			psld = (StrLogData*)ple->data;
//			printf("4020 lsh=%s\tcpkey=%s\n",psld->lsh,psld->cpkey);
//			ple = list_next(ple);
//		}
		printf("before 4020 len=%d\t4030 len=%d\n",list_size(&list4020),list_size(&list4030));
		//break;
		
		//delete keyFile
		if(0==access(keyFile,F_OK)) remove(keyFile);
		
		compareLogRecords(&list4020,&list4030,retFile,keyFile);
		printf("middle 4020 len=%d\t4030 len=%d\n",list_size(&list4020),list_size(&list4030));
		printf("compareLogRecords:%s over!\n",jvwgFile);
		i++;
		
	
	}
	
	list_destroy(&list4020);
	list_destroy(&list4030);
	printf("after 4020 len=%d\t4030 len=%d\n",list_size(&list4020),list_size(&list4030));
	
	exit(0);


}
