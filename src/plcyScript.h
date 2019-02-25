#ifndef PLCYSCRIPT_H
#define PLCYSCRIPT_H

//Policy Script Interpretor buffer length
#define PSI_BUFFER_LEN	512

//Service Policy structure
typedef struct _SRV_PLCY
{
	char srv_name[PSI_BUFFER_LEN];	//Service name
	char plcy_str[PSI_BUFFER_LEN];	//Relavant policy string
}SRV_PLCY, *PSRV_PLCY;

extern char gMachineName[];			//(global variables)indicated in plcyScript.y
extern char gMachineOrDomain[];		//(global variables)indicated in plcyScript.y
extern SRV_PLCY gPsiArray[];		//(global variables)indicated in plcyScript.y
extern int gPsiArrayIndex;			//(global variables)indicated in plcyScript.y

#endif
