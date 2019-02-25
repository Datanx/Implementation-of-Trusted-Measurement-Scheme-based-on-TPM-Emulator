/////////////////////////////////////////////////////////////////
//This file is the ".h" file of softinfo module                //
//                                                             //
//Computer Institute of Xidian University in Dec,2009          //
/////////////////////////////////////////////////////////////////
#ifndef		IN
#define		IN
#endif

#ifndef		OUT
#define		OUT
#endif

#ifndef GET_CLIENTSOFT_INFO

#define GET_CLIENTSOFT_INFO

////////////////////////////////////////
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

extern "C"
{
    #include "md5lib.h"
}
////////////////////////////////////////
#define BUFSIZE                   256
#define SOFTNAMESIZE              32
#define SHELLCOMMANDSIZE          64
#define VERSIONSIZE               32
#define HASHVALUESIZE             40
///////////////////////////////////////////////////
//where can be extended                          //
#define BASHSHELLVER_INDICT       "Bash"
#define LINUXVER_INDICT           "kernel"
#define GCCVER_INDICT             "Gcc"
#define FIREFOX_INDICT            "firefox"
#define OPENOFFICE_INDICT         "openoffice"
///////////////////////////////////////////////////
//where can be extended                          //
#define BASHSHELL_BASE            0x0001
#define LINUXVER_BASE             0x0002
#define GCCVER_BASE               0x0004
#define FIREFOX_BASE              0x0008
#define OPENOFFICE_BASE           0x0010
///////////////////////////////////////////////////
//where can be extended                          //
#define BASHSHELL_COMMAND_FIRST   "which bash"
#define BASHSHELL_COMMAND_SECOND  "  --version"

#define LINUXVER_COMMAND          "cat /proc/version"

#define GCCVER_COMMAND_FIRST      "which gcc"
#define GCCVER_COMMAND_SECOND     "  --version"

#define FIREFOX_COMMAND          "firefox --version"

#define OPENOFFICE_COMMAND       "openoffice.org -?"
///////////////////////////////////////////////////
typedef struct MainSoftInfo
{
   char softname[SOFTNAMESIZE];
   char version[VERSIONSIZE];
   char md5_hash[HASHVALUESIZE];
} TMainSoftInfo,*PTMainSoftInfo;

typedef struct CurRecord_SoftSave
{
   int  Isorno_success;
   TMainSoftInfo mainsoftinfo;
} T_SoftSave,*PT_SoftSave;

typedef struct Record_SoftInfo
{
   unsigned int softID;
   char psoftname[SOFTNAMESIZE];
   char pcommand[SHELLCOMMANDSIZE];
   char RecordBuf[BUFSIZE];
   int  len_info;
   TMainSoftInfo mainsoftinfo;
   struct Record_SoftInfo *next;
} TRecord_SoftInfo,*PTRecord_SoftInfo;

//using namespace std;

class CSoftInfo
{
public:
   PT_SoftSave  p_softsave;
   PTRecord_SoftInfo  p_recordsoftinfo;
   
   CSoftInfo();
   ~CSoftInfo();
   void inits_SoftInfo(); 
   void retrieve_struct();
   void Set_SoftCommand(PTRecord_SoftInfo pRec,const char *P_SetCommand);
   int  Get_SoftInfo(char *P_GetCommand,char *P_info,int *Pn_info);
private:
   void Softname_swap(char *pstname,unsigned int i_softname);
   void version_shortcut(unsigned int i_softname,IN char *pstversion,OUT char *pshortstversion);
   
   int BashShellInfo(unsigned int Int_base,PTRecord_SoftInfo pRec_i);
   int LinuxVerInfo(unsigned int Int_base,PTRecord_SoftInfo pRec_i);
   int GccVerInfo(unsigned int Int_base,PTRecord_SoftInfo pRec_i);
   int FirefoxInfo(unsigned int Int_base,PTRecord_SoftInfo pRec_i);
   int OpenOfficeInfo(unsigned int Int_base,PTRecord_SoftInfo pRec_i);

////////////////////////////////////////////////////////////////////  
// where can be extended but individual function define be added  //
////////////////////////////////////////////////////////////////////

   int SoftWareInfo(unsigned int base,PTRecord_SoftInfo pprecord);
public:
   void write_hash_MainSoftInfo(PTMainSoftInfo pmsoftinfo,char *in_name,char *in_ver);
   int Get_SoftRecord(unsigned int int_softname);
   void SwitchPrint(unsigned int funcname_ID);

   void SerializeSoftInfo(IN PTMainSoftInfo pmainsoftinfo, OUT char *out_str, OUT int *out_str_len);
   void UnserializeSoftStruct(OUT PTMainSoftInfo pmainsoftinfo, IN char *in_str);
 //  void MultiSwitchPrint(unsigned int multifuncname_ID); 
};
/////////////////////////////////////////////////////

#endif
