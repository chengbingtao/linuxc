/*
serverV1
简单的tcp client程序
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

int main(int argc,char** argv)
{
	struct in_addr inAddr;
	struct sockaddr_in sockAddr,cliAddr;
	int iRet=0;
	char c[3];
	int listenFd,connFd;	
	socklen_t len;

	//iRet = inet_aton(INADDR_ANY);
	bzero(&sockAddr,sizeof(sockAddr));
	sockAddr.sin_family=AF_INET;
	iRet = inet_aton("127.0.0.1",&inAddr);
	sockAddr.sin_addr = inAddr;
	sockAddr.sin_port = htons(9527);
	
	listenFd = socket(AF_INET,SOCK_STREAM,0);
	printf("socket return %d\n",listenFd);
	//bind(listenFd,(struct sockaddr*)&sockAddr,sizeof(sockAddr));
	//listenFd = socket(AF_INET,SOCK_STREAM,0);
	iRet = connect(listenFd,(struct sockaddr*)&sockAddr,sizeof(sockAddr));
	if(iRet <0)
	{
		printf("connect return:%d\n",iRet);
		printf("error:%s\n",strerror(errno));
	}	
	write(listenFd,"222",3);
	read(listenFd,c,3);
	printf("c=%s\n",c);
	close(listenFd);
	//listen(listenFd,17);

	//while(true)
	//{
	//	connFd = accept(listenFd,(struct sockaddr*)&cliAddr,&len);
	//	write(connFd,"123",3);
	//	close(connFd);
	//}

}
