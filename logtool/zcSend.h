#include<stdlib.h>
#include<stdio.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include <fcntl.h>
#include<stdbool.h>
#include<errno.h>
#ifndef ZCSEND
#define ZCSEND
//控制是否编译异地灾备版本的宏
#define CICS_SQL_SEQ						";"
#define CICS_REDIS_CMD					"REDIS"
#define CICS_MAIN_DB2						1

pthread_mutex_t errInfoLock;
int iErrInfoHandle;
char errInfoFile[1024];

typedef struct sendcontent_{
	char* ip;
	int   port;
	char buff[2048];
	int   len;
	char recv[2048];
	int   recv_len;
} SEND_CONTENT;

int sendbuff(int sock,char *sendmsg,int send_len);
int recvbuff(int sock,char *recvmsg,int *recv_len);
int init_socket(char *ip,unsigned int port);
int cics_redis_data(int sockfd,char *business_name,char* list, int draw,void* data, int sql_len, char* timestr,int direction,char *station_terminal,char *trans_serial);
bool is_socket_closed(int clientSocket);
void sendbuff2(SEND_CONTENT* psc);
#endif