#include "zcSend.h"


/*
write的包裹函数
当被信号中断后，继续写入，不受影响
返回写入字节数
*/
size_t Send(int fd,void* pOut,size_t sizeOut)
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

int init_socket(char *ip,unsigned int port)
{
	int sock;
	struct sockaddr_in serveraddr;
	int ret = 0;
	int err =0;
	int bDontLinger = -1;
	//这里由于桂强server返回的只有错误没有其他信息长度应该足够用
	//select 模型，即设置超时
	struct timeval time_out;
	fd_set r;
	unsigned long ul = 1;	
	unsigned long ul1= 0 ;
	
	int len = sizeof(int);
	int error = 0;
	//char recvmsg[1024] = {'\0'};
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(0 >= sock)
	{
		printf("socket return:%d,err:%s",sock,strerror(errno));
		return -1;
	}
	
	
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(ip);
	serveraddr.sin_port = htons((short)port);
	
	ret = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (0 > ret)
	{
			FD_ZERO(&r);
			FD_SET(sock, &r);
			time_out.tv_sec = 0; //连接超时3秒
			time_out.tv_usec =500*1000;
		  if( select(sock+1, NULL, &r, NULL, &time_out) > 0)
      {
         // getsockopt(sock, SOL_SOCKET, SO_ERROR, (char *)&error, /*(socklen_t *)*/&len);
          if(error != 0) ret = -5;	
      } 
      else
      {
          ret = -6;
      }
      if(ret!=0)
      {
      	//logn("connect is error,error:%d,ret=%d",error,ret);
				close(sock);
				return ret;
      }

	}
	
	//一般非阻塞模式套接比较难控制，可以根据实际情况考虑 再设回阻塞模式

	
	
	return sock;
}

int sendbuff(int sock,char *sendmsg,int send_len)
{
	//int total = 0;
	int slen = 0;
	int err = 0;

	if(send_len==0)
	{
		send_len = strlen(sendmsg);
	}

	//while(total < send_len)
	//{
		if ((slen=Send(sock, sendmsg, send_len )) < 0)
		{
			printf("socket send:%d,err:%s",slen,strerror(errno));
			return -1;
		}

		//total += slen;	
		//logn("total:%d,slen:%d",total,slen);
	//}	
	printf("send ok!\n");
	return slen;
}

int recvbuff(int sock,char *recvmsg,int *recv_len)
{
	int rlen = 0;
	int err = 0;

	if (0 >= recv_len || !recvmsg)
	{
		return -1;
	}

	//rlen= recv(sock,recvmsg, *recv_len,0);
	rlen = read(sock,recvmsg,*recv_len);
//	logn("recvmsg:%s", recvmsg);
	printf("read return %d\n",rlen);
	if(rlen < 0)
	{
		//err = WSAGetLastError();
		//logn("err:%d,line:%d",err,__LINE__);
		printf("socket read:%d,err:%s",rlen,strerror(errno));
		return -2;
	}

	*(recvmsg + rlen) = '\0';
	*recv_len = rlen;
	return (*recv_len);
}

int recvpack(int sock, char* recvmsg, int* recv_len)
{
	int				rlen = 0;
	int 			have_recv = 0;
	int			  buf_size = 0;
	int				datasize = 0;
	
	if (!recvmsg || !recv_len || 6 >= *recv_len)
	{
		return -1;
	}
	
	buf_size = *recv_len;
	
	while (6 > have_recv)
	{
		*recv_len = 6 - have_recv;
		rlen = recvbuff(sock, recvmsg + have_recv, recv_len);
		
		if (rlen <= 0)
		{
			return -1;
		}
		
		have_recv += rlen;
	}
	
	if ('@'!= *recvmsg || '|' != *(recvmsg + 5))
	{
		//logn("recvpack failed, err packet header, buf = %s", recvmsg);
		return -2;
	}
	
	datasize = atoi(recvmsg + 1);
	
	if (datasize + 6 >= buf_size)
	{
		//logn("recvpack failed, no enough buf, hope[%d], fact[%d]", datasize + 6, buf_size);
		return -3;
	}
	
	have_recv -= 6;
	
	while (datasize > have_recv)
	{
		*recv_len = datasize - have_recv;
		rlen = recvbuff(sock, recvmsg + 6 + have_recv, recv_len);
		
		if (rlen <= 0)
		{
			return -4;
		}
		
		have_recv += rlen;
	}
	
	*recv_len = have_recv + 6;
	*(recvmsg + *(recv_len)) = '\0';
	return (*recv_len);
}


int cics_redis_data(int sockfd,char *business_name,char* list, int draw,void* data, int sql_len, char* timestr,int direction,char *station_terminal,char *trans_serial)
{
	//int         sockfd = -1;
	char        packdata[10240] = {0};
	char        ans[512] = {0};
	int					len = 0;
  char*       p = NULL;
  long				rand_digital=0;
	char str_year[5]={0};
	char str_mon[3] = {0};
	char str_day[3] = {0};
	char str_hour[3] = {0};
	char str_min[3]={0};
	char str_sec[3]={0};
	char str_msec[4]={0};
	char str_time[64] = {0};
	
  memcpy(str_year,timestr,4);
	memcpy(str_mon,timestr+5,2);
	memcpy(str_day,timestr+8,2);
	memcpy(str_hour,timestr+11,2);
	memcpy(str_min,timestr+14,2);
	memcpy(str_sec,timestr+17,2);
	memcpy(str_msec,timestr+20,3);
	srand((unsigned)time(NULL));
	rand_digital = rand()%RAND_MAX;//取五位随机数
	//logn("[%s-%05d]",str_msec,rand_digital);
	//logn("rand:%05d",rand_digital);
	//logn("data:%s,len:%d",(char *)data,sql_len);
	sprintf(str_time,"%s-%s-%s %s:%s:%s",str_year,str_mon,str_day,str_hour,str_min,str_sec);
	sprintf(trans_serial,"%s%s%s%s%s%s%s%s%s%05d",business_name,station_terminal,str_year,str_mon,str_day,str_hour,str_min,str_sec,str_msec,rand_digital);	
	//logn("tran_serial:%s",trans_serial);
	/*为了保证中彩三大系统都是用同一个socket 2017-08-18
	if (0 > (sockfd = init_socket(ip, port)))
	{
		return -1;
	}
	*/
	memset(packdata, 0, sizeof(packdata));
	//由于协议修改”包头“在命令字后面添加一个随机数字段cics这里固定1即可
	if(draw==1)
	{
		//DATABASE_FLAG=0 Draw=1 异步(写REDIS)  draw=0 同步(转JAVA网关) DATABASE_FLAG ！=0  draw以下值 1：SQL语句 2：中间层存储过程3：后台存储过程
		len += sprintf(packdata + 6, "%d|%s|1|%s$%d$%d$", 1, CICS_REDIS_CMD, list, CICS_MAIN_DB2, draw);
	}
	else
	{
		//异地灾备已经测试过,此时容灾数据和实时上传数据协议有变化,通用函数只能做些处理！
		//DATABASE_FLAG=0 Draw=1 异步(写REDIS)  draw=0 同步(转JAVA网关) DATABASE_FLAG ！=0  draw以下值 1：SQL语句 2：中间层存储过程3：后台存储过程
		len += sprintf(packdata + 6, "%d|%s|1|%s$%d$%d$", 1, CICS_REDIS_CMD, list, 0, 1);
	}
	memcpy(packdata + len + 6, data, sql_len);
	len += sql_len;
	len += sprintf(packdata + len + 6, "$%s$%s$%d", str_time,trans_serial, direction);

	if (len >= 10000)
	{
	  //logn("cics_redis_sql  data to long, sqlsize = %d", sql_len);
		close(sockfd);
		return -1;
	}	
	
	sprintf(packdata, "@%04d", len);
	*(packdata + 5) = '|';
	len += 6;

	//PrintStatus(packdata);

	if (0 > sendbuff(sockfd, packdata, len))
	{
		//logn("send err:%d", WSAGetLastError());
		close(sockfd);
		return -2;
	}
	
//		logn("send = %s", packdata);
	memset(ans, 0, sizeof(ans));
	len = sizeof(ans);
	
	if (0 >= (len = recvpack(sockfd, ans, &len)))
	{
		//logn("recv err:%d",WSAGetLastError());
		//logn("cics_redis_data recv failed, req = %s, ans = %s", packdata, ans);
	  close(sockfd);
		return -3;
	}
	//为了保证中彩三大系统都是用同一个socket进行通信这里不进行关闭socket 2017-08-18
	//closesocket(sockfd);
//	logn("buf = %s", packdata);

  if (!(p = strrchr(ans, '|')) || 0 != atoi(p + 1))
  {
  	//logn("cics_redis_data ans failed, req = %s, ans = %s", packdata, ans);
  	return -2;
  }
  
	return 0;
}

