/*
serverV2
简单的tcp client程序
add function Write
add fork
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
#include<sys/wait.h>
/*
write的包裹函数
当被信号中断后，继续写入，不受影响
返回写入字节数
*/
size_t Write(int fd,void* pOut,size_t sizeOut)
{
	void* pTmp=NULL;
	int iPos=0;
	int iWt=0;
	pTmp=pOut;
	while(iPos < sizeOut)
	{
		iWt=write(fd,pTmp,sizeOut-iPos);
		if(iWt<0)
		{
			if(errno==EINTR)iPos=0;

			return -1;
		}else{
			iPos += iWt;
		}
		pTmp += iWt;
	}

	return iPos;
}

int main(int argc,char** argv)
{
	struct in_addr inAddr;
	struct sockaddr_in sockAddr,cliAddr;
	int iRet=0;
	char c[3];
	char pInput[10];
	int listenFd,connFd,i;	
	socklen_t len;
	pid_t pid;

	//iRet = inet_aton(INADDR_ANY);
	bzero(&sockAddr,sizeof(sockAddr));
	sockAddr.sin_family=AF_INET;
	iRet = inet_aton("127.0.0.1",&inAddr);
	sockAddr.sin_addr = inAddr;
	sockAddr.sin_port = htons(9527);
	
	for(i=0;i<500;i++)
	{
		if(fork()==0)
		{
			listenFd = socket(AF_INET,SOCK_STREAM,0);
			//printf("socket return %d\n",listenFd);
			//bind(listenFd,(struct sockaddr*)&sockAddr,sizeof(sockAddr));
			//listenFd = socket(AF_INET,SOCK_STREAM,0);
			iRet = connect(listenFd,(struct sockaddr*)&sockAddr,sizeof(sockAddr));
			if(iRet <0)
			{
				printf("connect return:%d\n",iRet);
				printf("error:%s\n",strerror(errno));
			}	
			//从终端读入

			//printf("input string please:\n");
			//pInput = malloc(10);
			//scanf("%s",pInput);
			sprintf(pInput,"%010d",i);
			//printf("thank you\n");
			
			write(listenFd,pInput,strlen(pInput));
			//free(pInput);
			//pInput=NULL;
			read(listenFd,c,3);
			printf("c=%s\n",c);
			close(listenFd);
			exit(0);
		}else
		{	//一定要wait啊，否则子进程好像不销毁，会找1号init进程当父进程，程序会很慢很慢，系统资源消耗的相当严重
			pid=wait(NULL);
			//此处会发现每次执行该程序打印出的一系列子进程id会比上次大，但ps查看不到之前进程号，之前进程应该被正确销毁了，但系统可能采用延迟重用算法，新创建的进程号会不同于刚销毁的进程号
			printf("%d process quit\n",pid);
		}
	}
	//listen(listenFd,17);

	//while(true)
	//{
	//	connFd = accept(listenFd,(struct sockaddr*)&cliAddr,&len);
	//	write(connFd,"123",3);
	//	close(connFd);
	//}

}
