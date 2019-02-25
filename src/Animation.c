#include "Animation.h"

void RotatePrint(unsigned int second, char* fout, int sout)
{
	int i = 0;

	char *ch1 = NULL;
	char *ch2 = NULL;
	char *ch3 = NULL;
	char *ch4 = NULL;

	int flen = 0;
	int slen = 0;
	int len = 0;
	flen = strlen(fout);
	slen = IntToLen(second);

	char *str_bak = NULL;
	char *str_cls = NULL;
	char *str_sec = NULL;
	char *str_for = NULL;

	if(sout == SOW_ON)
	{
		len = 2*slen + 2 + flen;
		str_bak = malloc((slen+2)*sizeof(char));
		bzero(str_bak,slen+2);
		str_cls = malloc((slen+2)*sizeof(char));
		bzero(str_cls,slen+2);
		str_sec = malloc((slen+1)*sizeof(char));
		bzero(str_sec,(slen+1));
		str_for = malloc(4*sizeof(char));
		bzero(str_for,4);
		for(i = 0; i < slen+1; i++)
		{
			strcat(str_bak,"\b");
			strcat(str_cls,".");
		}
	}	
	else
	{	
		str_bak = "\b";
		str_cls = " ";
		len = flen + 2;
	}

	ch1 = malloc((len+1)*sizeof(char));
	ch2 = malloc((len+1)*sizeof(char));
	ch3 = malloc((len+1)*sizeof(char));
	ch4 = malloc((len+1)*sizeof(char));

	fprintf(stdout,str_cls);

	for(i = 0; i < second*5; i++)
	{
		if(sout == SOW_ON)
		{
			sprintf(str_for,"%s%d%s","%",slen,"d");
			sprintf(str_sec,str_for,(second-i/5-1));
			sprintf(ch1,"%s%s%s%s",str_bak,fout,str_sec,CH1);
			sprintf(ch2,"%s%s%s%s",str_bak,fout,str_sec,CH2);
			sprintf(ch3,"%s%s%s%s",str_bak,fout,str_sec,CH3);
			sprintf(ch4,"%s%s%s%s",str_bak,fout,str_sec,CH4);
		}
		else
		{
			sprintf(ch1,"%s%s%s",str_bak,fout,CH1);
			sprintf(ch2,"%s%s%s",str_bak,fout,CH2);
			sprintf(ch3,"%s%s%s",str_bak,fout,CH3);
			sprintf(ch4,"%s%s%s",str_bak,fout,CH4);
		}

		fprintf(stdout,ch1);
		fflush(stdout);
		usleep(50000);
		fprintf(stdout,ch2);
		fflush(stdout);
		usleep(50000);
		fprintf(stdout,ch3);
		fflush(stdout);
		usleep(50000);
		fprintf(stdout,ch4);
		fflush(stdout);
		usleep(50000);
	}
	
	fprintf(stdout,"%s%s",str_bak,str_cls);

	free(ch1);
	free(ch2);
	free(ch3);
	free(ch4);	

	if(sout == SOW_ON)
	{
		free(str_bak);
		free(str_sec);
		free(str_for);
		free(str_cls);
	}
}

int IntToLen(unsigned int Integer)
{
	int i= 1;
	while(Integer/(i*10) != 0)
	{
		i++;
	}
	return i;
}
