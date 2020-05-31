#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biTree.h"
#include "ftpTool.h"
#include "zcSend.h"
#include "list.h"
#include "outFile.h"
//#define LINE_MAX 1024
#define PROFILE "profile.ini"

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


int getLogRecords( BiTree *successTree,char* logFileArray[])
//int getLogRecords(char* logFileArray[])
{
return 0;
}


void destroyPiao(void* data){
	struct piao *piao = (struct piao*)data;
	free(piao);
}

int main(int argc, char* argv[]){
	
	int iRet=0;
	int iRet2=0;
	char c[10];
	List listOut;	
	long lRet;

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
	
	list_init(&listOut,destroyPiao);
	lRet = getTicket("./outfile/20200531/2019001lotfs0000.out",&listOut);
	
	
	exit(0);


}
