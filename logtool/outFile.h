#ifndef OUTFILE
#define OUTFILE
#include "list.h"
#include "biTree.h"
struct lsnr/* ��ˮ��Ͷע���ݽṹ */
{
	int	xszbm,xsqh,lsh,tzs;
	char	cpkey[22];
	int		dnz,cpbz,tzlx,xsms;
	int   	dmsl,tmsl;//song+
	int		tz[8];
	int   	lsdt;//song+
	int   	ifdq;//song+
};

struct fsnr/* ��ʽ��Ͷע���ݽṹ */
{
	int	xszbm,xsqh,lsh,tzs;
	char	cpkey[22];
	int		dnz,cpbz,tzlx,xsms;
	int		dmsl,tmsl;
	int		tz[49];
	int		lsdt;//song+
	int 	ifdq;//song+
};

//�븴ʽ�ṹ��ͬ����dnz��Ϊ����
struct piao{
	int	xszbm,xsqh,lsh,tzs;
	char	cpkey[22];
	int		tiaoshu,cpbz,tzlx,xsms;
	int		dmsl,tmsl;
	int		tz[49];
	int		lsdt;//song+
	int 	ifdq;//song+
	
};

long getTicket(char* FileName,List *listRet);
unsigned long compareTicket(char* FileName,BiTree *keyTree);
unsigned long sendXkjRz(char* piaoFile,char *ip,unsigned int port,long* xszbmZdh[]);
#endif