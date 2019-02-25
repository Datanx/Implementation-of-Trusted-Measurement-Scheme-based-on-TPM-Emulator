#include <stdio.h>
#include <string.h>

#include <tss/tss_error.h>
#include <tss/platform.h>  
#include <tss/tss_defines.h>  
#include <tss/tss_typedef.h>  
#include <tss/tss_structs.h>
#include <tss/tss_error_basics.h>  
#include <tss/tspi.h>
#include <trousers/trousers.h>
#include <tss/tpm.h>
#include <trousers/tss.h>


#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>     // for socket
#include <sys/socket.h>    // for socket
#include <sys/time.h>      // for caculate time

#include <stdlib.h>        // for exit
#include <time.h>          //for time_t and time

const char *get_error(TSS_RESULT res) 
{ 
switch(ERROR_CODE(res))
 {
 case 0x0001L:				return "Authentication failed"; 
case TSS_E_BAD_PARAMETER:		return "TSS_E_BAD_PARAMETER"; 
case TSS_E_INTERNAL_ERROR:		return "an error occurred internal to the TSS"; 
case TSS_E_PS_KEY_NOTFOUND:		return "TSS_E_PS_KEY_NOTFOUND"; 
case TSS_E_KEY_ALREADY_REGISTERED:	return "TSS_E_KEY_ALREADY_REGISTERED"; 	
case TSS_E_CANCELED:			return "TSS_E_CANCELED"; 	
case TSS_E_TIMEOUT:			return "TSS_E_TIMEOUT"; 	
case TSS_E_OUTOFMEMORY:			return "TSS_E_OUTOFMEMORY"; 	
case TSS_E_TPM_UNEXPECTED:		return "TSS_E_TPM_UNEXPECTED"; 	
case TSS_E_COMM_FAILURE:		return "TSS_E_COMM_FAILURE";
case TSS_E_TPM_UNSUPPORTED_FEATURE:	return "TSS_E_TPM_UNSUPPORTED_FEATURE"; 
case TSS_E_INVALID_OBJECT_TYPE:		return "TSS_E_INVALID_OBJECT_TYPE"; 
case TSS_E_INVALID_OBJECT_INITFLAG:	return "TSS_E_INVALID_OBJECT_INITFLAG"; 
case TSS_E_INVALID_HANDLE:		return "TSS_E_INVALID_HANDLE"; 	
case TSS_E_NO_CONNECTION:		return "TSS_E_NO_CONNECTION"; 	
case TSS_E_CONNECTION_FAILED:		return "TSS_E_CONNECTION_FAILED"; 	
case TSS_E_CONNECTION_BROKEN:		return "TSS_E_CONNECTION_BROKEN";
 case TSS_E_PS_KEY_EXISTS:		return "TSS_E_PS_KEY_EXISTS"; 
case TSS_E_INVALID_ATTRIB_FLAG:		return "attribflag is incorrect"; 
case TSS_E_INVALID_ATTRIB_SUBFLAG:	return "subflag is incorrect"; 
case TSS_E_INVALID_ATTRIB_DATA:		return "ulAttrib is incorrect";
 case TSS_E_KEY_NOT_LOADED:		return "TSS_E_KEY_NOT_LOADED"; 	 	
case TSS_E_KEY_NOT_SET:			return "TSS_E_KEY_NOT_SET"; 	
case TSS_E_VALIDATION_FAILED:		return "TSS_E_VALIDATION_FAILED"; 	
case TSS_E_TSP_AUTHREQUIRED:		return "TSS_E_TSP_AUTHREQUIRED"; 	
case TSS_E_TSP_AUTH2REQUIRED:		return "TSS_E_TSP_AUTH2REQUIRED"; 	
case TSS_E_TSP_AUTHFAIL:		return "TSS_E_TSP_AUTHFAIL"; 	
case TSS_E_TSP_AUTH2FAIL:		return "TSS_E_TSP_AUTH2FAIL"; 	
case TSS_E_KEY_NO_MIGRATION_POLICY:	return "TSS_E_KEY_NO_MIGRATION_POLICY"; 	
case TSS_E_POLICY_NO_SECRET:		return "TSS_E_POLICY_NO_SECRET"; 	
case TSS_E_INVALID_OBJ_ACCESS:		return "TSS_E_INVALID_OBJ_ACCESS"; 	
case TSS_E_INVALID_ENCSCHEME:		return "TSS_E_INVALID_ENCSCHEME"; 	
case TSS_E_INVALID_SIGSCHEME:		return "TSS_E_INVALID_SIGSCHEME"; 	
case TSS_E_ENC_INVALID_LENGTH:		return "TSS_E_ENC_INVALID_LENGTH"; 	
case TSS_E_ENC_NO_DATA:			return "TSS_E_ENC_NO_DATA"; 	
case TSS_E_ENC_INVALID_TYPE:		return "TSS_E_ENC_INVALID_TYPE"; 	
case TSS_E_INVALID_KEYUSAGE:		return "TSS_E_INVALID_KEYUSAGE"; 	
case TSS_E_VERIFICATION_FAILED:		return "TSS_E_VERIFICATION_FAILED"; 	 
case TSS_SUCCESS:			return "success"; 	
default: return "unknown error"; 
} 
} 




int bindData(TSS_UUID hKeyUUID,int dataLen,char *dataToBind)
{ 
TSS_RESULT result; 
TSS_HTPM hTPM;
TSS_HCONTEXT hContext; 
TSS_HPOLICY hOwnerPolicy,hSRKPolicy,hUsagePolicy,hMigPolicy; 	
TSS_UUID SRK_UUID=TSS_UUID_SRK; 
TSS_HKEY hSRK,hKey; 	
TSS_HENCDATA hEncData; 
TSS_FLAG   initFlags=TSS_KEY_TYPE_BIND|TSS_KEY_SIZE_2048|TSS_KEY_NO_AUTHORIZATION|TSS_KEY_MIGRATABLE; 
BYTE *MigratePubKey = NULL;
UINT32 MigratePubKeySize = 0; 	
BYTE  *rgbBoundData = NULL;
UINT32  ulDataLength = 0;
BYTE wks[20]; 
memset(wks,0,20);
 /* step 1. Create Context and Connect */ 
//Create Context 	
result =Tspi_Context_Create(&hContext); 	
if(result!=TSS_SUCCESS) 	
{ 	
printf("Tspi_Context_Create ERROR:%s(%04x)\n",get_error(result),result); 	
Tspi_Context_Close(hContext); 	
return -1; 	
} 	
// Pick the TPM you are talking to in this case the system TPM (which you connect to with
result= Tspi_Context_Connect(hContext,NULL); 	
if(result!=TSS_SUCCESS) 	
{ 	
printf("Tspi_Context_Connect ERROR:%s(%04x)\n",get_error(result),result); 	
Tspi_Context_Close(hContext); 	
return -1; 	
} 
/* step 2. Set TPM Policy and SRK Policy */ 	
//Get the TPM Object 	
result= Tspi_Context_GetTpmObject(hContext,&hTPM); 	
if(result!=TSS_SUCCESS) 	
f(result!=TSS_SUCCESS) 	
{ 	
printf("Tspi_Context_GetTpmObject ERROR:%s(%04x)\n",get_error(result),result); 	
Tspi_Context_Close(hContext); 	
return -1; 	
} 	
//Get the TPM Policy 	
result=Tspi_GetPolicyObject(hTPM,TSS_POLICY_USAGE,&hOwnerPolicy); 	
if(result!=TSS_SUCCESS) 	
{ 	
printf("Tspi_GetPolicyObject ERROR:%s(%04x)\n",get_error(result),result); 	
Tspi_Context_Close(hContext); 	
return -1; 	
} 	
//Set secret for TPM Policy 	
result=Tspi_Policy_SetSecret(hOwnerPolicy,TSS_SECRET_MODE_SHA1,20,wks); 	
if(result!=TSS_SUCCESS) 	
{ 	
printf("Tspi_Policy_SetSecret ERROR:%s(%04x)\n",get_error(result),result); 	
Tspi_Context_Close(hContext); 	
return -1; 	
}
 /* step 3. Load the SRK */ 	
//Load the SRK by its UUID 	
result= Tspi_Context_LoadKeyByUUID(hContext,TSS_PS_TYPE_SYSTEM,SRK_UUID,&hSRK); 
if(result!=TSS_SUCCESS) 	
{ 	 
printf("Tspi_Context_LoadKeyByUUID ERROR:%s(%04x)\n",get_error(result),result); 	
Tspi_Context_Close(hContext); 		
return -1; 	
} 	
//Get the SRK Policy 	
result= Tspi_GetPolicyObject(hSRK,TSS_POLICY_USAGE,&hSRKPolicy); 	
if(result!=TSS_SUCCESS) 	
{ 	
printf("Tspi_GetPolicyObject ERROR:%s(%04x)\n",get_error(result),result); 	
Tspi_Context_Close(hContext); 	
return -1; 	
} 	
//Set secret for SRK Policy 	
result= Tspi_Policy_SetSecret(hSRKPolicy,TSS_SECRET_MODE_SHA1,20,wks); 	
if(result!=TSS_SUCCESS) 	
{ 	
printf("Tspi_Policy_SetSecret ERROR:%s(%04x)\n",get_error(result),result); 	
Tspi_Context_Close(hContext); 	
return -1; 	
} 	 	 	
 /* step 4. Create a KeyObject named hKey to Wrap Migrated Key */ 	
result = Tspi_Context_CreateObject( hContext, TSS_OBJECT_TYPE_RSAKEY,initFlags, &hKey );			 	
if ( result != TSS_SUCCESS ) 	
{ 		
printf("Tspi_Context_CreateObject (hkey) ERROR:%s(%04x)\n",get_error(result),result); 	
Tspi_Context_Close(hContext); 	
return -1; 	
} 	
result=Tspi_Context_CreateObject(hContext,TSS_OBJECT_TYPE_POLICY,TSS_POLICY_USAGE,&hUsagePolicy);	 	
if ( result != TSS_SUCCESS ) 
{ 		
printf("Tspi_GetPolicyObject (hUsagePolicy) ERROR:%s(%04x)\n",get_error(result),result); 	
Tspi_Context_Close(hContext); 	
return -1; 	
}			 	 	
//Set Secret 	
result = Tspi_Policy_SetSecret( hUsagePolicy,TSS_SECRET_MODE_SHA1,20,wks); 	
if ( result != TSS_SUCCESS ) 
{ 		
printf("Tspi_Policy_SetSecret (hUsagePolicy) ERROR:%s(%04x)\n",get_error(result),result); 	
Tspi_Context_Close(hContext); 	
return -1; 	
} 	
result = Tspi_Policy_AssignToObject(hUsagePolicy,hKey); 	
result=Tspi_Context_CreateObject(hContext,TSS_OBJECT_TYPE_POLICY,TSS_POLICY_MIGRATION,&hMigPolicy);
f ( result != TSS_SUCCESS ) 	
{ 		
printf("Tspi_GetPolicyObject (hMigPolicy) ERROR:%s(%04x)\n",get_error(result),result); 	
Tspi_Context_Close(hContext); 	
return -1; 	
} 	
//Set Secret 	
result = Tspi_Policy_SetSecret( hMigPolicy , TSS_SECRET_MODE_SHA1,20,wks); 	
if ( result != TSS_SUCCESS )
{ 		
printf("Tspi_Policy_SetSecret (hMigPolicy) ERROR:%s(%04x)\n",get_error(result),result); 	
Tspi_Context_Close(hContext); 	
return -1; 	
}	 	
result = Tspi_Policy_AssignToObject(hMigPolicy, hKey); 	
if(result!=TSS_SUCCESS) 	
{ 	
printf("Tspi_Policy_AssignToObject(hMigPolicy)ERROR:%s(%04x)\n",get_error(result),result); 	
Tspi_Context_Close(hContext); 	
return -1; 	
} 	
 /* step 5.  	create a migrate bind key */	
result = Tspi_Key_CreateKey(hKey,hSRK,0); 		
if(result!=TSS_SUCCESS) 		
{ 			
printf("Tspi_Key_CreateKey ERROR:%s(%04x)\n",get_error(result),result); 			
Tspi_Context_Close(hContext); 			
return -1; 		
} 		
printf("Register the bind key into TPM......\n"); 		
result=Tspi_Context_RegisterKey(hContext,hKey,TSS_PS_TYPE_SYSTEM,hKeyUUID,TSS_PS_TYPE_SYSTEM,SRK_UUID); 		
if(result!=TSS_SUCCESS) 		
{ 			
if(ERROR_CODE(result)==TSS_E_KEY_ALREADY_REGISTERED) 			
{ printf("Warning:UUID has been used......\n"); }			 			
} 
//load the bind key into TPM 		
result=Tspi_Key_LoadKey(hKey,hSRK); 		
if(result!=TSS_SUCCESS) 		
{ 			
printf("Tspi_Key_LoadKey ERROR:%s(%04x)\n",get_error(result),result); 			
Tspi_Context_Close(hContext); 			
return -1; 		
} 	
 
/* step 6. Bind data */	 	
result = Tspi_Context_CreateObject(hContext,TSS_OBJECT_TYPE_ENCDATA,TSS_ENCDATA_BIND,&hEncData);	 	
if(result!=TSS_SUCCESS) 
{ 
printf("Tspi_Context_CreateObject (encData) ERROR:%s(%04x)\n",get_error(result),result); 
Tspi_Context_Close(hContext); 
return -1; 
} 		
 result = Tspi_Data_Bind( hEncData, hKey, dataLen,dataToBind); 	
if(result!=TSS_SUCCESS)
 { 
printf("Tspi_Data_Bind ERROR:%s(%04x)\n",get_error(result),result); 
Tspi_Context_Close(hContext); 
return -1; 
} 	 	
result=Tspi_GetAttribData(hEncData,TSS_TSPATTRIB_ENCDATA_BLOB,TSS_TSPATTRIB_ENCDATABLOB_BLOB, &ulDataLength,&rgbBoundData); 	
if(result!=TSS_SUCCESS) 
{ 
printf("Tspi_GetAttribData ERROR:%s(%04x)\n",get_error(result),result); 
Tspi_Context_Close(hContext); 
return -1;
 } 	
printf("ulDataLength=%d\n",ulDataLength); 
//(*encDataSize)=ulDataLength; 	
//(*encData)=(char *)malloc(ulDataLength*sizeof(char)); 	
//memcpy((*encData),rgbBoundData,ulDataLength); 

/* *STEP 7 . UnloadKey */ 	
result=Tspi_Key_UnloadKey(hKey); 	
if(result!=TSS_SUCCESS)
{ 
printf("Tspi_Key_UnloadKey ERROR:%s(%04x)\n",get_error(result),result); 
Tspi_Context_Close(hContext); 
return -1; 
} 
/* *step 8. clean up */	
// Clean up 	
Tspi_Context_FreeMemory(hKey, NULL); 
Tspi_Context_FreeMemory(hContext, NULL); 
Tspi_Context_Close(hContext); 
return 0; 	 	
} 

#define NUM 5
int main()
{
TSS_UUID hKeyUUID={0x74525622,0x45ce,0x4f5a,0x9a,0xb4,{0x6e,0x60,0x71,0xb9,0xb7,0xaf}};
int i,k,size=0;
struct timeval tpstart,tpend;
double timeuse;
char TypePass[NUM];
for(k = 0; k < NUM ; k++)
	TypePass[k] = 'a';
        size=sizeof(TypePass);
        printf("sizeof(TypePass)=%d B\n",size);
//int *encDataSize;
//char **encData;
gettimeofday(&tpstart,NULL);
i=bindData(hKeyUUID,strlen(TypePass),TypePass);
gettimeofday(&tpend,NULL);
timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
printf("used time:%fus\n",timeuse);
return 0;
}
