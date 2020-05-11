/*
serverV2
简单的tcp服务程序
*/

#include<stdlib.h>
#include<stdio.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<stdbool.h>
#include<errno.h>

/*
读取文件描述符，到pIn，读取sizeIn个字节
用于网络通信，当遇到中断时继续读取
返回总共读取字节数
*/
int Read(int fd,void* pIn,size_t sizeIn)
{
	void* pTmp=NULL;
	int iRead=0;
	int iCnt=0;
	int iPos=0;
	while(sizeIn - iCnt > 0)
	{
		pTmp=malloc(sizeIn);
		memset(pTmp,0,sizeIn);
		iRead = read(fd,pTmp,sizeIn);

		if(iRead==0) return iCnt;
		if(iRead == -1){
			if(errno==EINTR)
				iRead=0;
			else
				return -1;
		}
		//if(iRead==0) return iCnt;

		if(iRead>0) iCnt += iRead;

		memcpy(pIn+iPos,pTmp,iCnt);
		iPos+=iCnt;
		free(pTmp);
	}
	return iCnt;
	
}

int main(int argc,char** argv)
{
	struct in_addr inAddr;
	struct sockaddr_in sockAddr,cliAddr;
	int iRet=0;
	int listenFd,connFd;	
	socklen_t len;
	char c[10];
	//iRet = inet_aton(INADDR_ANY);
	bzero(&sockAddr,sizeof(sockAddr));
	sockAddr.sin_family=AF_INET;
	
//服务器程序不必指定ip
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//sockAddr.sin_addr.s_addr = htonl("127.0.0.1");
	//iRet=inet_aton("192.168.31.106",&inAddr);
	//iRet=inet_aton("127.0.0.1",&inAddr);

	//if(iRet<0){
	//	printf("inet_aton return:%d,addr=%d\n",iRet,inAddr.s_addr);
	//}
	//sockAddr.sin_addr = inAddr;
	sockAddr.sin_port = htons(9527);
	
	listenFd = socket(AF_INET,SOCK_STREAM,0);
	if(listenFd<0){
//		系统调用，需要调用strerror函数，将errno转换为具体信息
		printf("socket return:%d,err:%s",listenFd,strerror(errno));
		return -1;
	}
	iRet = bind(listenFd,(struct sockaddr*)&sockAddr,sizeof(sockAddr));
	//listenFd = socket(AF_INET,SOCK_STREAM,0);
	if(iRet<0){
			printf("bind return %d,err:%s\n",iRet,strerror(errno));
			return -1;
		}

	//此处14是参照书中表格，在linux中，表示正在连接和已经连接的队列内连接数量上限是17个
	iRet = listen(listenFd,14);
	if(iRet < 0){
		printf("listen return %d,err:%s",iRet,strerror(errno));
		return -1;
	}
	//len是传入的参数，并非传出的，所以需要传入具体值
	len=sizeof(cliAddr);
	while(true)
	{
		connFd = accept(listenFd,(struct sockaddr*)&cliAddr,&len);
		printf("accept addr:%d\n",cliAddr.sin_addr.s_addr);
		printf("accepted\n");
		memset(c,0,sizeof(c));
		//iRet = read(connFd,c,3);
		iRet=Read(connFd,c,10);
		if(iRet<0){
			printf("read return %d,recevie:%s\n",iRet,c);
			printf("error:%s\n",strerror(errno));
		}
		printf("receive success:%s\n",c);
		write(connFd,"123",3);
		close(connFd);
	}

}

