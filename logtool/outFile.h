#ifndef OUTFILE
#define OUTFILE
#include "list.h"
struct lsnr/* 流水库投注内容结构 */
{
	long	xszbm,xsqh,lsh,tzs;
	char	cpkey[22];
	int		dnz,cpbz,tzlx,xsms;
	int   	dmsl,tmsl;//song+
	int		tz[8];
	int   	lsdt;//song+
	int   	ifdq;//song+
};

struct fsnr/* 复式库投注内容结构 */
{
	long	xszbm,xsqh,lsh,tzs;
	char	cpkey[22];
	int		dnz,cpbz,tzlx,xsms;
	int		dmsl,tmsl;
	int		tz[49];
	int		lsdt;//song+
	int 	ifdq;//song+
};

//与复式结构相同，仅dnz改为条数
struct piao{
	long	xszbm,xsqh,lsh,tzs;
	char	cpkey[22];
	int		tiaoshu,cpbz,tzlx,xsms;
	int		dmsl,tmsl;
	int		tz[49];
	int		lsdt;//song+
	int 	ifdq;//song+
	
};

long getTicket(char* FileName,List *listRet);


#endif