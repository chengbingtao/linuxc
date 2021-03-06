#include <stdio.h>
#include "outFile.h"
#include "list.h"
#include "chengTools.h"
#include "biTree.h"
#include "zcSend.h"
#include "threadpool.h"

#define XKJ_SEND 0
#define CommandID "REDIS"

/*
将流水内容复制到票内容，会影响效率，便于程序维护
*/
int lscopy(struct lsnr* ls,struct piao *piao){
	piao->xszbm =ls->xszbm;
	piao->xsqh = ls->xsqh;
	piao->lsh = ls->lsh;
	piao->tzs = ls->tzs;
	if(NULL==strcpy(piao->cpkey,ls->cpkey))return -1;
	//piao->dnz = ls->dnz;
	piao->cpbz = ls->cpbz;
	piao->tzlx = ls->tzlx;
	piao->xsms = ls->xsms;
	piao->dmsl=ls->dmsl;
	piao->tmsl = ls->tmsl;
	//0-9 第一注；10-19第二注；。。。40-49第五注
	if(1 <=ls->dnz && 5>= ls->dnz)
		if(NULL==memcpy(piao->tz + (ls->dnz - 1) * 10,ls->tz,sizeof(int) * 8)) return -1;
	piao->lsdt=ls->lsdt;
	piao->ifdq = ls->ifdq;
	
	return 0;
}


int fscopy(struct fsnr* fs,struct piao *piao){
	if(memcpy(piao,fs,sizeof(struct piao)) == NULL) return -1;
	
	return 0;
}

long getTicket(char* FileName,List *listRet)
{
	int  	i;
	
	unsigned long FileLength,fl;
	char 	keyZhu[22];
	long 	lastdis,ttt;
	int		FileNo;
	FILE	*lsk=NULL;
	int lsOrFs=-1;
	int tiaoshu=0;
	int zhushu=0;
	struct	lsnr sthout1;
	struct	fsnr sthout2;
	struct  piao *strPiao;
	
	
	FileLength = get_file_size(FileName);
	//流水库文件读取并处理
	if(strstr(FileName,"ls") || strstr(FileName,"LS") ){
		lsOrFs=1;
		if((FileLength%sizeof(sthout1))!=0) 
		{ 
			printf("%s%s:%d:%d\n","Error length of ",FileName,FileLength,sizeof(sthout1));
			//printf("%s\n",err_text);
			//Deal_Err(err_text);
			return -2; 
		}
		fl=FileLength/sizeof(sthout1);		/*流水库文件单条记录长度是80*/
		//printf("The file length of %s is %ld\n",FileName,FileLength);
	}
	//复式库文件读取并处理
	if(strstr(FileName,"fs") || strstr(FileName,"FS") ){
		lsOrFs=2;
		if((FileLength%sizeof(sthout2))!=0) 
		{ 
			printf("%s%s:%d:%d\n","Error length of ",FileName,FileLength,sizeof(sthout2));
			//printf("%s\n",err_text);
			//Deal_Err(err_text);
			return -2; 
		}
		fl=FileLength/sizeof(sthout2);		/*流水库文件单条记录长度是80*/
		//printf("The file length of %s is %ld\n",FileName,FileLength);
	}
	
	if(lsOrFs== -1) return -1;

	
	FileLength=fl;
	
	/* 循环读取流水库文件 */
	lastdis=0;
	
	if((lsk = fopen(FileName,"rb"))==NULL) return -3;
	for(ttt=0;ttt<FileLength;ttt++){
			
	
			if(lsOrFs==1){
				memset(&sthout1,0,sizeof(sthout1));
				fread(&sthout1,sizeof(sthout1),1,lsk);
				//printf("lsk cpk:%s\n",sthout1.cpkey);
			}	
			if(lsOrFs==2){
				memset(&sthout2,0,sizeof(sthout2));
				fread(&sthout2,sizeof(sthout2),1,lsk);
				//printf("fsk cpk:%s\n",sthout2.cpkey);
				//strcpy(keyZhu,sthout2.cpkey);
			}
			
			
			if(ttt > 0){
				if(lsOrFs==1){
					if(strcmp(strPiao->cpkey,sthout1.cpkey) != 0){
						//票结构体
						strPiao->tiaoshu = tiaoshu;
						strPiao->tzs = zhushu;
						tiaoshu=0;
						zhushu=0;
						//记录该票到存储结构
						//printf("piao key:%s\n",strPiao->cpkey);
						i=list_insert_next(listRet,NULL, strPiao);
						if(i==-1) return -4;
						lastdis++;	
					}
				}
				if(lsOrFs==2){
					if(strcmp(strPiao->cpkey,sthout2.cpkey) != 0){
						//票结构体
						strPiao->tiaoshu = tiaoshu;
						strPiao->tzs = zhushu;
						tiaoshu=0;
						zhushu=0;
						//记录该票到存储结构
						//printf("piao key:%s\n",strPiao->cpkey);
						i=list_insert_next(listRet,NULL, strPiao);
						if(i==-1) return -4;
						lastdis++;	
					}	
				}
				
			}
			
			if(lsOrFs==1){
					memset(keyZhu,0,sizeof(keyZhu));
					strPiao = malloc(sizeof(struct piao));
					memset(strPiao,0,sizeof(struct piao));
					//strcpy(keyZhu,sthout1.cpkey);
					if(-1 == lscopy(&sthout1,strPiao)){
						//记录一下，但不需要停
						
					}
					zhushu += sthout1.tzs; 
			}
			
			if(lsOrFs == 2){
					memset(keyZhu,0,sizeof(keyZhu));
					strPiao = malloc(sizeof(struct piao));
					memset(strPiao,0,sizeof(struct piao));
					if(-1 == fscopy(&sthout2,strPiao)){
						//记录一下
					}
					zhushu += sthout2.tzs;
			}
			//条数
			tiaoshu++;	
				
		}
		//最后一票
		strPiao->tiaoshu = tiaoshu;
		
		//记录该票到存储结构
		//printf("piao key:%s\n",strPiao->cpkey);
		i=list_insert_next(listRet,NULL, strPiao);			
		if(i==-1) return -5;	
		lastdis++;			
		if(lsk )fclose(lsk);
	
	
	
	//返回总票数
	return lastdis;
}

/*
compare the records of out file named FileName,and BiTree's node,if equal,send and delete tree's node
*/
unsigned long compareTicket_(char* FileName,BiTree *keyTree,char* tgtFile){
	int  	i;
	
	unsigned long FileLength,fl;
	char 	keyZhu[22];
	long 	lastdis,ttt;
	int		FileNo;
	
	FILE	*lsk=NULL;
	FILE  *fpTgt=NULL;
	int lsOrFs=-1;
	int tiaoshu=0;
	int zhushu=0;
	struct	lsnr sthout1;
	struct	fsnr sthout2;
	struct  piao strPiao;
	BiTreeNode *pNode=NULL;
	
	
	FileLength = get_file_size(FileName);
	if(0==FileLength) return 0;
	//流水库文件读取并处理
	if(strstr(FileName,"ls") || strstr(FileName,"LS") ){
		lsOrFs=1;
		if((FileLength%sizeof(sthout1))!=0) 
		{ 
			printf("%s%s:%d:%d\n","Error length of ",FileName,FileLength,sizeof(sthout1));
			//printf("%s\n",err_text);
			//Deal_Err(err_text);
			return -2; 
		}
		fl=FileLength/sizeof(sthout1);		/*流水库文件单条记录长度是80*/
		//printf("The file length of %s is %ld\n",FileName,FileLength);
	}
	//复式库文件读取并处理
	if(strstr(FileName,"fs") || strstr(FileName,"FS") ){
		lsOrFs=2;
		if((FileLength%sizeof(sthout2))!=0) 
		{ 
			printf("%s%s:%d:%d\n","Error length of ",FileName,FileLength,sizeof(sthout2));
			//printf("%s\n",err_text);
			//Deal_Err(err_text);
			return -2; 
		}
		fl=FileLength/sizeof(sthout2);		/*流水库文件单条记录长度是80*/
		//printf("The file length of %s is %ld\n",FileName,FileLength);
	}
	
	if(lsOrFs== -1) return -1;

	
	FileLength=fl;
	
	/* 循环读取流水库文件 */
	lastdis=0;
	
	if((lsk = fopen(FileName,"rb"))==NULL) return -3;
		
	if(0==access(tgtFile,F_OK)) remove(tgtFile);
		
	if((fpTgt = fopen(tgtFile,"wb")) == NULL) return -4;	
		
	//printf("line:245\n");		
	
		
	for(ttt=0;ttt<FileLength;ttt++){
		
		if(lsOrFs==1){
				memset(&sthout1,0,sizeof(sthout1));
				fread(&sthout1,sizeof(sthout1),1,lsk);
				//printf("lsk cpk:%s\n",sthout1.cpkey);
			}	
			if(lsOrFs==2){
				memset(&sthout2,0,sizeof(sthout2));
				fread(&sthout2,sizeof(sthout2),1,lsk);
				//printf("fsk cpk:%s\n",sthout2.cpkey);
				//strcpy(keyZhu,sthout2.cpkey);
			}
			
			if(ttt > 0){
				if(lsOrFs==1){
					if(strcmp(strPiao.cpkey,sthout1.cpkey) != 0){
						//票结构体
						strPiao.tiaoshu = tiaoshu;
						strPiao.tzs = zhushu;
						tiaoshu=0;
						zhushu=0;
						//记录该票到存储结构
						//printf("piao key:%s\n",strPiao->cpkey);
						//i=list_insert_next(listRet,NULL, strPiao);
						//if(i==-1) return -4;
						pNode = search(keyTree, strPiao.cpkey);
						if(pNode){
							//printf("%s\t",pNode->data);
							deleteNode2(keyTree, pNode);
						}else{
							//printf("send javawg:write to fpTgt 3,key=%s\n",strPiao->cpkey);
							//send javawg
							if((fwrite(&strPiao,sizeof(struct piao),1,fpTgt))!=1) 
								printf("write to %s error:zdh=%d,key=%s\n",tgtFile,strPiao.xszbm,strPiao.cpkey);
						}
						//printf("piao content:key=%s;ts=%d,zs=%d,tz=",strPiao.cpkey,strPiao.tiaoshu,strPiao.tzs);
						//for(i=0;i<49;i++) printf("%d,",strPiao.tz[i]);
						//printf("\n");
						//strPiao = malloc(sizeof(struct piao));
						memset(&strPiao,0,sizeof(struct piao));
						lastdis++;	
					}
				}
				if(lsOrFs==2){
					if(strcmp(strPiao.cpkey,sthout2.cpkey) != 0){
						//票结构体
						strPiao.tiaoshu = tiaoshu;
						strPiao.tzs = zhushu;
						tiaoshu=0;
						zhushu=0;
						//记录该票到存储结构
						//printf("piao key:%s\n",strPiao->cpkey);
//						i=list_insert_next(listRet,NULL, strPiao);
//						if(i==-1) return -4;
							pNode = search(keyTree, strPiao.cpkey);
							if(pNode){
								
								//printf("%s\t",pNode->data);
								deleteNode2(keyTree, pNode);
							}else{
								//printf("send javawg:write to fpTgt 2,key=%s\n",strPiao->cpkey);
								//send javawg
								if((fwrite(&strPiao,sizeof(struct piao),1,fpTgt))!=1) 
								printf("write to %s error:zdh=%d,key=%s\n",tgtFile,strPiao.xszbm,strPiao.cpkey);
							}
							//strPiao = malloc(sizeof(struct piao));
							memset(&strPiao,0,sizeof(struct piao));

						lastdis++;	
					}	
				}
				
			}
			
			if(lsOrFs==1){
					memset(keyZhu,0,sizeof(keyZhu));
					//strPiao = malloc(sizeof(struct piao));
					//memset(strPiao,0,sizeof(struct piao));
					//strcpy(keyZhu,sthout1.cpkey);
					if(-1 == lscopy(&sthout1,&strPiao)){
						//记录一下，但不需要停
						
					}
					zhushu += sthout1.tzs; 
					
			}
			
			if(lsOrFs == 2){
					memset(keyZhu,0,sizeof(keyZhu));
					//strPiao = malloc(sizeof(struct piao));
					//memset(strPiao,0,sizeof(struct piao));
					if(-1 == fscopy(&sthout2,&strPiao)){
						//记录一下
					}
					zhushu += sthout2.tzs;
			}
			//条数
			tiaoshu++;
	}
	//最后一票
		strPiao.tiaoshu = tiaoshu;
		strPiao.tzs = zhushu;
		//记录该票到存储结构
		//printf("piao key:%s\n",strPiao->cpkey);
		//i=list_insert_next(listRet,NULL, strPiao);			
		//if(i==-1) return -5;
			
		pNode = search(keyTree, strPiao.cpkey);
		if(pNode){
			//printf("%s\t",pNode->data);
			deleteNode2(keyTree, pNode);
			
		}else{
			//printf("send javawg:write to fpTgt 1,key=%s\n",strPiao->cpkey);
			//send javawg
			if((fwrite(&strPiao,sizeof(struct piao),1,fpTgt))!=1) 
								printf("write to %s error:zdh=%d,key=%s\n",tgtFile,strPiao.xszbm,strPiao.cpkey);
								
			//printf("piao content:key=%s;ts=%d,zs=%d,tz=",strPiao.cpkey,strPiao.tiaoshu,strPiao.tzs);
			//for(i=0;i<49;i++) printf("%d,",strPiao.tz[i]);
			//printf("\n");					
		}
			
		lastdis++;	
			
		if(lsk )fclose(lsk);
	
		if(fpTgt) fclose(fpTgt);
			
		lsk=NULL;
		fpTgt=NULL;	
	
	
	//返回总票数
	return lastdis;
}

//给目标文件加上玩法
unsigned long compareTicket(char* FileName,BiTree *keyTree){
	char cWf[5];
	char tgtFile[1024];
	//char newTgtFile[1024];
	memset(cWf,0,sizeof(cWf));
	//4位玩法
//	if(strstr(FileName,"lot"))  strcpy(cWf,"lot "); 
//	if(strstr(FileName,"c730")) strcpy(cWf,"c730");
//	if(strstr(FileName,"c736")) strcpy(cWf,"c736");
//	if(strstr(FileName,"c522")) strcpy(cWf,"c522");
//	if(strstr(FileName,"p62"))  strcpy(cWf,"p62 ");
//	if(strstr(FileName,"p3"))   strcpy(cWf,"p3  ");
	
	
		
	sprintf(tgtFile,"%s_N",FileName);
	
	return compareTicket_(FileName,keyTree,tgtFile);						
}

//新开奖
unsigned long sendXkjRz(char* piaoFile,char *ip,unsigned int port,long xszbmZdh[])
{
	FILE *fp = NULL;
	FILE *fpSendBack=NULL;
	int ifdSendBack=0;
	unsigned long FileLength,fl,ttt;
	struct piao  strPiao;
	char sendBackFile[1024];
	char xkj[1024];
	char rz[1024];
	char xkj2[1024];
	char rz2[1024];
	//char Body[2048];
	char sendbuf[1024];
	long Xszbm8;
	char cWf[5];
	char cYxbm[6];
	char *p=NULL;
	char Code[200];
	char cDate[11],cTime[9],cDatetime[20];
	char cDatetimeSimple[20];
	char cDate6[7];
	char ywlsh[20];
	time_t now;
	struct tm *timeNow;
	size_t readSize=0;
	//char CommandID[12],Serial[10];
	char Serial[10];
	unsigned long SerialNum;
	int iSeriaNum=0;
	int i,ifLsFs,h_dmsl,h_tmsl;
	int iSocket=0,iRet=0,iLen;
	char recvmsg[2048];
	long lRet;
	struct my_threadpool* pThreadPool=NULL;
	SEND_CONTENT *par_send_content;
	FileLength = get_file_size(piaoFile);
	//流水库文件读取并处理
	if(0==FileLength) return 0;
	
	if((FileLength%sizeof(struct piao))!=0) 
	{ 
		printf("%s%s:%d:%d\n","Error length of ",piaoFile,FileLength,sizeof(struct piao));
			//printf("%s\n",err_text);
			//Deal_Err(err_text);
		return -1; 
	}
	fl=FileLength/sizeof(struct piao);		/*流水库文件单条记录长度是80*/
		//printf("The file length of %s is %ld\n",FileName,FileLength);
	
	
	
		memset(cWf,0,sizeof(cWf));
		memset(cYxbm,0,sizeof(cYxbm));
		if(strstr(piaoFile,"lot"))  {
			strcpy(cWf,"lot");
			strcpy(cYxbm,"10001");
		} 
		if(strstr(piaoFile,"c730")) {
			strcpy(cWf,"c730");
			strcpy(cYxbm,"10003");
		}
		if(strstr(piaoFile,"c736")) {
			strcpy(cWf,"c736");
			strcpy(cYxbm,"23002");
		}
				
		if(strstr(piaoFile,"c522")) {
			strcpy(cWf,"c522");
			strcpy(cYxbm,"23001");
		}
		if(strstr(piaoFile,"p62"))  {
			strcpy(cWf,"p62");
			strcpy(cYxbm,"23003");
		}
		if(strstr(piaoFile,"p3"))  { 
			strcpy(cWf,"p3");
			strcpy(cYxbm,"10002");
		}
	
		if(strstr(piaoFile,"ls")) ifLsFs = 1;
		if(strstr(piaoFile,"fs")) ifLsFs = 2;	

	
	FileLength=fl;
	printf("%d tickets in file %s\n",FileLength,piaoFile);
	fp = fopen(piaoFile,"rb");
	
	
	
	memset(cDatetime,0,sizeof(cDatetime));
	if( getSystemTime( cDatetime)){
		memset(cDate,0,sizeof(cDate));
		strncpy(cDate,cDatetime,10);
		
		memset(cTime,0,sizeof(cTime));
		p=strstr(cDatetime," ");
		strcpy(cTime,p+1);
	}
	
	time(&now);
	SerialNum = (unsigned long)now;
	timeNow = localtime(&now);
	sprintf(cDatetimeSimple,"%d%02d%02d%02d%02d%02d",timeNow->tm_year+1900,timeNow->tm_mon+1,timeNow->tm_mday,timeNow->tm_hour,timeNow->tm_min,timeNow->tm_sec);
	sprintf(cDate6,"%02d%02d%02d",timeNow->tm_year-100,timeNow->tm_mon+1,timeNow->tm_mday);
	
	//sprintf(sendBackFile,"./%s.send",cDate);
	//fpSendBack = fopen(sendBackFile,"wb");
	//if(fpSendBack <=0){
	//	printf("%s file fopen error:%s\n",sendBackFile,strerror(errno));
	//}
	sprintf(sendBackFile,"./%s.send2",cDate);
	
	ifdSendBack = open(sendBackFile,O_WRONLY|O_CREAT|O_APPEND);
	if(ifdSendBack == -1){
		printf("%s file open error:%s\n",sendBackFile,strerror(errno));
	}
//	fwrite(cDate,strlen(cDate),1,fpSendBack);
//	fwrite(cDate,strlen(cDate),1,fpSendBack);
//	fwrite(cDate,strlen(cDate),1,fpSendBack);
	//iSocket = init_socket(ip,port);
	//if(iSocket <=0) return -2;
	
	//printf("481\n");
	
	pThreadPool = threadpool_init(10, 100);
	
	pthread_mutex_init(&errInfoLock, NULL);
	memset(errInfoFile,0,sizeof(errInfoFile));
	sprintf(errInfoFile,"./%s.sendfail",cDate);
	iErrInfoHandle = open(errInfoFile,O_WRONLY|O_CREAT);
	for(ttt=0;ttt<FileLength;ttt++){
		
		memset(&strPiao,0,sizeof(strPiao));
		memset(xkj,0,sizeof(xkj));
		memset(rz,0,sizeof(rz));
		memset(Code,0,sizeof(Code));
		
		//iSocket = init_socket(ip,port);
		//if(iSocket <=0) return -2;
		
		fread(&strPiao,sizeof(strPiao),1,fp);
		
//				for(lRet =39999;lRet>=0;lRet--){
//					if(xszbmZdh[lRet] > 0) printf("<%d>\t",xszbmZdh[lRet]);
//				}
//				printf("\n%d\n",xszbmZdh[30679]);
		
				//printf("lsk zdh:%d\n",strPiao.xszbm);
		Xszbm8 = xszbmZdh[strPiao.xszbm];
		
		sprintf(ywlsh,"23%s%05d%05d",cDate6,strPiao.xszbm,strPiao.lsh % 100000);
		
		
		if(ifLsFs == 1){
			for(i=0;i<strPiao.tiaoshu;i++){
				
				if(i!=0)	sprintf(Code+strlen(Code),"%s","#");
					
				if(strcmp(cWf,"p3")==0)//
					sprintf(Code+strlen(Code),"00&01&%04d&03%02d%02d%02d",strPiao.tz[i*10+6],strPiao.tz[i*10+0],strPiao.tz[i*10+1],strPiao.tz[i*10+2]);
				if(strcmp(cWf,"lot")==0)
					sprintf(Code+strlen(Code),"00&01&%04d&06%02d%02d%02d%02d%02d%02d01%02d",
						strPiao.tzs / strPiao.tiaoshu,strPiao.tz[i*10+0],strPiao.tz[i*10+1],strPiao.tz[i*10+2],strPiao.tz[i*10+3],strPiao.tz[i*10+4],strPiao.tz[i*10+5],strPiao.tz[i*10+6]);
				if(strcmp(cWf,"c730")==0)
					sprintf(Code+strlen(Code),"00&01&%04d&07%02d%02d%02d%02d%02d%02d%02d",
						strPiao.tzs / strPiao.tiaoshu,strPiao.tz[i*10+0],strPiao.tz[i*10+1],strPiao.tz[i*10+2],strPiao.tz[i*10+3],strPiao.tz[i*10+4],strPiao.tz[i*10+5],strPiao.tz[i*10+6]);
				if(strcmp(cWf,"c736")==0)
					sprintf(Code+strlen(Code),"00&01&%04d&07%02d%02d%02d%02d%02d%02d%02d",
						strPiao.tzs / strPiao.tiaoshu,strPiao.tz[i*10+0],strPiao.tz[i*10+1],strPiao.tz[i*10+2],strPiao.tz[i*10+3],strPiao.tz[i*10+4],strPiao.tz[i*10+5],strPiao.tz[i*10+6]);	
				if(strcmp(cWf,"c522")==0)
					sprintf(Code+strlen(Code),"00&01&%04d&05%02d%02d%02d%02d%02d",
						strPiao.tzs / strPiao.tiaoshu,strPiao.tz[i*10+0],strPiao.tz[i*10+1],strPiao.tz[i*10+2],strPiao.tz[i*10+3],strPiao.tz[i*10+4]);	
				if(strcmp(cWf,"p62")==0)
					sprintf(Code+strlen(Code),"00&01&%04d&06%02d%02d%02d%02d%02d%02d01%02d",
						strPiao.tzs / strPiao.tiaoshu,strPiao.tz[i*10+0],strPiao.tz[i*10+1],strPiao.tz[i*10+2],strPiao.tz[i*10+3],strPiao.tz[i*10+4],strPiao.tz[i*10+5],strPiao.tz[i*10+6]);	
				
					
			}
					
			if((strcmp(cWf,"lot")==0)||(strcmp(cWf,"c730")==0))
			{
				if(XKJ_SEND){	
						//新开奖
						//Xszbm8需要转换,SerialNum需要实现
					sprintf(xkj,"ND_Region_List$0$1$4020,%s,23,%s,%d,%d,1,%d,%d,,%d,1,,,,,,,,,%s %s,%d.00,,1,1,1,%d,%s$%s %s$HLJHT%s%03d11111$4",
					       strPiao.cpkey,cYxbm,strPiao.xsqh,strPiao.xsqh,strPiao.xsqh,Xszbm8,1,cDate,cTime,strPiao.tzs*2,strPiao.tiaoshu,Code,cDate,cTime,cDatetimeSimple,SerialNum % 1000);
													//THead(sendbuf,Body);	
													
					if(strPiao.cpbz==0)
						sprintf(xkj2,"ND_Region_List$0$1$4020,%s,23,%s,%d,%d,1,%d,%d,,%d,1,,,,,,,,,%s %s,%d.00,,1,1,1,%d,%s$%s %s$HLJHT%s%03d11111$4",
					       strPiao.cpkey,cYxbm,strPiao.xsqh,strPiao.xsqh,strPiao.xsqh,Xszbm8,1,cDate,cTime,strPiao.tzs*2,strPiao.tiaoshu,Code,cDate,cTime,cDatetimeSimple,SerialNum % 1000);
				}								       
			}
			memset(rz,0,sizeof(rz));			
			sprintf(rz,"DT_Region_List$0$1$%s,%s,23,%s,%d,%d,1,%d,%d,%s,%d,1,,,,,,,,,%s %s,%d.00,,1,1,1,%d,%s$%s %s$HLJHT%s%03d11111$8",
							 "4020",strPiao.cpkey,cYxbm,strPiao.xsqh,strPiao.xsqh,strPiao.xsqh,Xszbm8,ywlsh,1,cDate,cTime,strPiao.tzs*2,strPiao.tiaoshu,Code,cDate,cTime,cDatetimeSimple,SerialNum % 1000);
			
			memset(rz2,0,sizeof(rz2));	
			if(strPiao.cpbz==0)		
				sprintf(rz2,"DT_Region_List$0$1$%s,%s,23,%s,%d,%d,1,%d,%d,%s,%d,1,,,,,,,,,%s %s,%d.00,,1,1,1,%d,%s$%s %s$HLJHT%s%03d11111$8",
							 "4020",strPiao.cpkey,cYxbm,strPiao.xsqh,strPiao.xsqh,strPiao.xsqh,Xszbm8,ywlsh,1,cDate,cTime,strPiao.tzs*2,strPiao.tiaoshu,Code,cDate,cTime,cDatetimeSimple,SerialNum % 1000);
			
		}
			
		if(ifLsFs == 2){
			if(strcmp(cWf,"lot")==0)
		 	{
		  		h_dmsl=strPiao.tmsl/100;
		  		h_tmsl=strPiao.tmsl%100;	
		  		
		  		
		  			  		
		  		if(h_dmsl!=0)
		  		{
			  			sprintf(Code+strlen(Code),"00&03&0001&");
			  			sprintf(Code+strlen(Code),"%02d",h_dmsl);
			  			for(i=0;i<h_dmsl;i++)
			  			{
			  			  sprintf(Code+strlen(Code),"%02d",strPiao.tz[i]);
		  		 	  }
			  	}
				  else
				  		sprintf(Code+strlen(Code),"00&02&0001&");
				  		
				  sprintf(Code+strlen(Code),"%02d",h_tmsl);
				  for(i=0;i<h_tmsl;i++)
				  {
				  	 sprintf(Code+strlen(Code),"%02d",strPiao.tz[i+h_dmsl]);
				  }
				  sprintf(Code+strlen(Code),"%02d",strPiao.dmsl); 	
				  
				  
				    
				  for(i=0;i<strPiao.dmsl;i++)
				  {
				     sprintf(Code+strlen(Code),"%02d",strPiao.tz[i+h_dmsl+h_tmsl]);
				  }					 
			}
			else
			{	  		
				if(strPiao.dmsl!=0)//
				{
					sprintf(Code+strlen(Code),"00&03&0001&");//胆拖情况
					sprintf(Code+strlen(Code),"%02d",strPiao.dmsl);
					for(i=0;i<strPiao.dmsl;i++)
					{
				  	sprintf(Code+strlen(Code),"%02d",strPiao.tz[i]);
				  }
				}
				else
					sprintf(Code+strlen(Code),"00&02&0001&");//复式的时候
				  	  
				  sprintf(Code+strlen(Code),"%02d",strPiao.tmsl);
				  for(i=0;i<strPiao.tmsl;i++)
					{
				    sprintf(Code+strlen(Code),"%02d",strPiao.tz[i+strPiao.dmsl]);
				  } 
			} 
			  	
			if((strcmp(cWf,"lot")==0)||(strcmp(cWf,"c730")==0))
			{	
				if(XKJ_SEND){
					sprintf(xkj,"ND_Region_List$0$1$%s,%s,23,%s,%d,%d,1,%d,%d,,1,1,,,,,,,,,%s %s,%d.00,,1,1,1,1,%s$%s %s$HLJHT%s%03d11111$4",
						   "4020",strPiao.cpkey,cYxbm,strPiao.xsqh,strPiao.xsqh,strPiao.xsqh,Xszbm8,cDate,cTime,strPiao.tzs*2,Code,cDate,cTime,cDatetimeSimple,SerialNum % 1000);
								//THead(sendbuf,Body);	
					//printf("xkj=%s\n",xkj);			
					if(strPiao.cpbz == 0)
							sprintf(xkj2,"ND_Region_List$0$1$%s,%s,23,%s,%d,%d,1,%d,%d,,1,1,,,,,,,,,%s %s,%d.00,,1,1,1,1,%s$%s %s$HLJHT%s%03d11111$4",
						   "4020",strPiao.cpkey,cYxbm,strPiao.xsqh,strPiao.xsqh,strPiao.xsqh,Xszbm8,cDate,cTime,strPiao.tzs*2,Code,cDate,cTime,cDatetimeSimple,SerialNum % 1000);
				}
			}
			memset(rz,0,sizeof(rz));					
			sprintf(rz,"DT_Region_List$0$1$%s,%s,23,%s,%d,%d,1,%d,%d,%s,1,1,,,,,,,,,%s %s,%d.00,,1,1,1,1,%s$%s %s$HLJHT%s%03d11111$8",
					  				 "4020",strPiao.cpkey,cYxbm,strPiao.xsqh,strPiao.xsqh,strPiao.xsqh,Xszbm8,ywlsh,cDate,cTime,strPiao.tzs*2,Code,cDate,cTime,cDatetimeSimple,SerialNum % 1000);////
			
			memset(rz2,0,sizeof(rz2));
			if(strPiao.cpbz==0)
				sprintf(rz2,"DT_Region_List$0$1$%s,%s,23,%s,%d,%d,1,%d,%d,%s,1,1,,,,,,,,,%s %s,%d.00,,1,1,1,1,%s$%s %s$HLJHT%s%03d11111$8",
					  				 "4020",strPiao.cpkey,cYxbm,strPiao.xsqh,strPiao.xsqh,strPiao.xsqh,Xszbm8,ywlsh,cDate,cTime,strPiao.tzs*2,Code,cDate,cTime,cDatetimeSimple,SerialNum % 1000);////
					  				 
			//printf("rz=%s\n",rz);
			//printf("rz2=%s\n",rz2);		  		
		}
				
		//return 0;	
				
		if(strlen(xkj) > 0 && XKJ_SEND){
			//sprintf(CommandID,"%s","REDIS");
					
			sprintf(Serial,"%ld%04d",time(NULL),SerialNum);
			SerialNum++;	
					//if((SerialNum>=1000)||(SerialNum<0))	SerialNum=0;
			memset(sendbuf,0,sizeof(sendbuf));	
			sprintf(sendbuf,"@%04d|1|%s|%s|%s",
				4+strlen(CommandID)+strlen(Serial)+strlen(xkj),CommandID,Serial,xkj);
				
				
			//fputs(sendbuf,fpSendBack);	
			//fputs("\n",fpSendBack);
						
				//printf("socket send:%s\n",sendbuf);
				
				
				//memset(par_send_content,0,sizeof(SEND_CONTENT));
				par_send_content = calloc(1,sizeof(SEND_CONTENT));
				
				par_send_content->ip=ip;	//地址赋值
				par_send_content->port=port;
				strcpy(par_send_content->buff,sendbuf);
				par_send_content->len = strlen(sendbuf);
				
				i = threadpool_add_job(pThreadPool, sendbuff2, &par_send_content);
				
				
				//i=sendbuff(iSocket,sendbuf, strlen(sendbuf));
			if(0 > i ){
				printf("sendbuff error return %d:%s\n",i,sendbuf);
				continue;
			}else{
				printf("recieve:%s\n",par_send_content->recv);
			}
		}
		
		if(strlen(xkj2) > 0 && XKJ_SEND){
			//sprintf(CommandID,"%s","REDIS");
					
			sprintf(Serial,"%ld%04d",time(NULL),SerialNum);
			SerialNum++;	
					//if((SerialNum>=1000)||(SerialNum<0))	SerialNum=0;
			memset(sendbuf,0,sizeof(sendbuf));	
			
			sprintf(sendbuf,"@%04d|1|%s|%s|%s",
				4+strlen(CommandID)+strlen(Serial)+strlen(xkj2),CommandID,Serial,xkj2);
				
				
			//fputs(sendbuf,fpSendBack);	
			//fputs("\n",fpSendBack);			
				//printf("socket send:%s\n",sendbuf);
				
				par_send_content = calloc(1,sizeof(SEND_CONTENT));
				
				par_send_content->ip=ip;	//地址赋值
				par_send_content->port=port;
				strcpy(par_send_content->buff,sendbuf);
				par_send_content->len = strlen(sendbuf);
				
				i = threadpool_add_job(pThreadPool, sendbuff2, &par_send_content);
				
				
				
				
				//i=sendbuff(iSocket,sendbuf, strlen(sendbuf));
			if(0 > i ){
				printf("sendbuff error return %d:%s\n",i,sendbuf);
				continue;
			}else{
				printf("recieve:%s\n",par_send_content->recv);
			}
		}
				
		if(strlen(rz) > 0){
//			printf("commandid_before:%s\n",CommandID);
//			//sprintf(CommandID,"%s","REDIS");
//			memset(CommandID,0,sizeof(CommandID));
//			printf("commandid_midd:%s\n",CommandID);
//			strcpy(CommandID,"REDIS");
					
			sprintf(Serial,"%ld%04d",time(NULL),iSeriaNum++);
			if(iSeriaNum > 9999) iSeriaNum = 0;
			//SerialNum++;	
				
			memset(sendbuf,0,sizeof(sendbuf));	
			sprintf(sendbuf,"@%04d|1|%s|%s|%s",
				4+strlen(CommandID)+strlen(Serial)+strlen(rz),CommandID,Serial,rz);
			
		//	printf("rz:%s\n",sendbuf);
			//fputs(sendbuf,fpSendBack);	
			//readSize = fwrite(sendbuf,strlen(sendbuf),1,fpSendBack);
			
			write(ifdSendBack,sendbuf,strlen(sendbuf));
			write(ifdSendBack,"\r\n",strlen("\r\n"));
			//printf("fwrite handle=%d return %d:%s\n",fpSendBack,readSize,strerror(errno));
			//fwrite("\n",1,1,fpSendBack);
		//	fprintf(fpSendBack,"%s\n",sendbuf);
			
			par_send_content = calloc(1,sizeof(SEND_CONTENT));
				
				par_send_content->ip=ip;	//地址赋值
				par_send_content->port=port;
				strcpy(par_send_content->buff,sendbuf);
				par_send_content->len = strlen(sendbuf);
				
				i = threadpool_add_job(pThreadPool, sendbuff2, par_send_content);
				
				
			
			if(0 > i){
				printf("sendbuff error return %d:%s\n",i,sendbuf);
				continue;
			}else{
				//printf("recieve:%s\n",par_send_content.recv);
				
			}
		}
		
		if(strlen(rz2) > 0){
			//sprintf(CommandID,"%s","REDIS");
					
			sprintf(Serial,"%ld%04d",time(NULL),SerialNum);
			SerialNum++;	
					
			memset(sendbuf,0,sizeof(sendbuf));	
			sprintf(sendbuf,"@%04d|1|%s|%s|%s",
				4+strlen(CommandID)+strlen(Serial)+strlen(rz2),CommandID,Serial,rz2);
			//printf("rz,file=%s:%s\n",piaoFile,sendbuf);
			//fputs(sendbuf,fpSendBack);	
			//fputs("\n",fpSendBack);

			par_send_content = calloc(1,sizeof(SEND_CONTENT));
				
				par_send_content->ip=ip;	//地址赋值
				par_send_content->port=port;
				strcpy(par_send_content->buff,sendbuf);
				par_send_content->len = strlen(sendbuf);
				
				i = threadpool_add_job(pThreadPool, sendbuff2, &par_send_content);
				
				
			if(0 > i){
				printf("sendbuff error return %d:%s\n",i,sendbuf);
				continue;
			}else{
				printf("recieve:%s\n",par_send_content->recv);
				
			}
		}
				
				
	}
	threadpool_destroy(pThreadPool);
	pthread_mutex_destroy(&errInfoLock);
	if(fp) fclose(fp);	
	if(ifdSendBack) close(ifdSendBack);
	if(iErrInfoHandle) close(iErrInfoHandle);		
	//if(fpSendBack) fclose(fpSendBack);
		
	return ttt;	
}


