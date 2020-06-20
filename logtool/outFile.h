#ifndef OUTFILE
#define OUTFILE
#include "list.h"
#include "biTree.h"
struct lsnr/* 流水库投注内容结构 */
{
	int	xszbm,xsqh,lsh,tzs;
	char	cpkey[22];
	int		dnz,cpbz,tzlx,xsms;
	int   	dmsl,tmsl;//song+
	int		tz[8];
	int   	lsdt;//song+
	int   	ifdq;//song+
};

struct fsnr/* 复式库投注内容结构 */
{
	int	xszbm,xsqh,lsh,tzs;
	char	cpkey[22];
	int		dnz,cpbz,tzlx,xsms;
	int		dmsl,tmsl;
	int		tz[49];
	int		lsdt;//song+
	int 	ifdq;//song+
};

//与复式结构相同，仅dnz改为条数
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