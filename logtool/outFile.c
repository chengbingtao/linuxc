#include <stdio.h>
#include "outFile.h"
#include "list.h"
#include "chengTools.h"

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
	if(NULL==memcpy(piao->tz,ls->tz,8)) return -1;
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
	
	//FileNo=open(FileName,0,0);
	//if(FileNo==-1) 
	//	return -6;
	//FileLength=filelength(FileNo);		/*读取文件长度*/
	
	
	///close(FileNo);
		
	
	
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
						strPiao->tiaoshu = zhushu;
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
						strPiao->tiaoshu = zhushu;
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
			}
			
			if(lsOrFs == 2){
					memset(keyZhu,0,sizeof(keyZhu));
					strPiao = malloc(sizeof(struct piao));
					memset(strPiao,0,sizeof(struct piao));
					if(-1 == fscopy(&sthout2,strPiao)){
						//记录一下
					}
					
			}
			//注数
			zhushu++;		
		}
		//最后一票
		strPiao->tiaoshu = zhushu;
		
		//记录该票到存储结构
		//printf("piao key:%s\n",strPiao->cpkey);
		i=list_insert_next(listRet,NULL, strPiao);			
		if(i==-1) return -5;	
		lastdis++;			
		if(lsk )fclose(lsk);
	
	
	
	//返回总票数
	return lastdis;
}
