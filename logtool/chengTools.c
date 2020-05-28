int readini(char *filename,char *PName,char *RName,char *result)
{
	FILE * fp;
	char buf[256],PName1[32],RName1[32];
	int s,i;
	char *pp;
	if ((fp = fopen(filename,"rt")) == NULL)
		return 0;
	sprintf(PName1,"[%s]",PName);
	sprintf(RName1,"%s=",RName);
	
	memset(buf,0,sizeof(buf));
	s=0;
	while(fgets(buf, sizeof(buf), fp))
	{
//		printf("%s",buf);
		if(strstr(buf,PName1)!=NULL)
		{	s=1;	continue;	}
		if(s==0)
			continue;
		else
		{
			if(strstr(buf,RName1)!=NULL)
			{
				strcpy(result,buf+strlen(RName1));
				for(i=0;i<strlen(result);i++)
				{
					if(result[i]==0x0d||result[i]==0x0a)
					{	result[i]=0;	break;	}
				}
				fclose(fp);
				return 1;
			}
		}
		if((strstr(buf,"[")!=NULL)&&(strstr(buf,"]")!=NULL))
		{
			s=0;
		}
		memset(buf,0,sizeof(buf));
	}
	fclose(fp);
	return 0;
}