/////////////////////////////////////////////////////////////////
//This module aims to retrieve information of diverse software //
//installed in current Linux operating system.                 //
//                                                             //
//Computer Institute of Xidian University in Dec,2009          //
/////////////////////////////////////////////////////////////////

#include "SoftInfo.h"

CSoftInfo::CSoftInfo()
{
   p_softsave=(PT_SoftSave)malloc(sizeof(struct CurRecord_SoftSave));
   p_softsave->Isorno_success=0;
//   memset(p_softsave->cursoftbuf,0,BUFSIZE+1);
   memset(&(p_softsave->mainsoftinfo),0,sizeof(p_softsave->mainsoftinfo));
   this->p_recordsoftinfo = NULL;    
}

void CSoftInfo::inits_SoftInfo()
{
   p_softsave=(PT_SoftSave)malloc(sizeof(struct CurRecord_SoftSave));
   p_softsave->Isorno_success=0;
//   memset(p_softsave->cursoftbuf,0,BUFSIZE+1);
   memset(&(p_softsave->mainsoftinfo),0,sizeof(p_softsave->mainsoftinfo));
   this->p_recordsoftinfo=NULL;
}

CSoftInfo::~CSoftInfo()
{
   PTRecord_SoftInfo pfreeRec,qfreeRec=NULL;
   if (p_softsave!=NULL)
      free(this->p_softsave);
   pfreeRec=p_recordsoftinfo;
   while (pfreeRec!=NULL)
   {
      qfreeRec=pfreeRec->next;
      free(pfreeRec);
      pfreeRec=qfreeRec;
   }
}

void CSoftInfo::retrieve_struct()
{
   PTRecord_SoftInfo pfreeRec,qfreeRec=NULL;
   if (p_softsave!=NULL)
      free(p_softsave);
   pfreeRec=p_recordsoftinfo;
   while (pfreeRec!=NULL)
   {
      qfreeRec=pfreeRec->next;
      free(pfreeRec);
      pfreeRec=qfreeRec;
   }
}

void CSoftInfo::Set_SoftCommand(PTRecord_SoftInfo pRec,const char *P_SetCommand)
{
   memset(pRec->pcommand,0,SHELLCOMMANDSIZE);
   strcpy(pRec->pcommand,P_SetCommand);
}

int CSoftInfo::Get_SoftInfo(char *P_GetCommand,char *P_info,int *Pn_info)
{
   FILE *fp;
   char filebuf[128];
   memset((void *)P_info,0, *Pn_info);
   if ((fp=popen(P_GetCommand,"r"))==NULL)
      perror("popen");
   rewind(fp);
   setbuffer(fp,filebuf,128);
   if ((fgets(P_info,*Pn_info,fp))==NULL)
       {
      printf("Shell command is incorrect！\n");
      return -1;
       }
/*   if (fp==NULL)
        {
      pclose(fp);
      printf("hello!\n");
      return -1;
        }
   if ((fread(P_info,sizeof(char),*Pn_info,fp))==0)
       {
      printf("Shell command is incorrect！\n");
      pclose(fp);
      return -1;
       }*/
   *Pn_info=strlen(P_info)-1;
   P_info[*Pn_info]='\0';   
   pclose(fp);
   return 0;
}

void CSoftInfo::Softname_swap(char *pstname,unsigned int i_softname)
{
    memset(pstname,0,SOFTNAMESIZE);
    switch(i_softname)
    {
        case BASHSHELL_BASE:
            strcpy(pstname,BASHSHELLVER_INDICT);
            break;
        case LINUXVER_BASE:
            strcpy(pstname,LINUXVER_INDICT);
            break;
        case GCCVER_BASE:
            strcpy(pstname,GCCVER_INDICT);
            break;
        case FIREFOX_BASE:
            strcpy(pstname,FIREFOX_INDICT);
            break;
        case OPENOFFICE_BASE:
            strcpy(pstname,OPENOFFICE_INDICT);
            break;
/////////////////////////////////// where can be extended            
    }
}

void CSoftInfo::version_shortcut(unsigned int i_softname,IN char *pstversion,OUT char *pshortstversion)
{
    if(pstversion == NULL)
        return;
    char *pstr = pstversion,*qstr=pshortstversion,*pt1,*pt2;
    memset(qstr,0,VERSIONSIZE);
    switch(i_softname)
         {
        case BASHSHELL_BASE:
            pt1=strstr(pstr,"version");
            pt1+=8;
            pt2=strchr(pt1,'(');
            pt2[0]='\0';
            strcpy(qstr,pt1);
            break;
        case LINUXVER_BASE:
            pt1=strstr(pstr,"version");
            pt1+=8;
            pt2=strchr(pt1,'(');
            pt2[0]='\0';
            strcpy(qstr,pt1);
            break;
        case GCCVER_BASE:
            pt1=pstr+10;
            pt2=strchr(pt1,' ');
            pt2[0]='\0';
            strcpy(qstr,pt1);
            break;
        case FIREFOX_BASE:
            pt1=strstr(pstr,"Firefox");
            pt1+=8;
            pt2=strchr(pt1,',');
            pt2[0]='\0';
            strcpy(qstr,pt1);
            break;
        case OPENOFFICE_BASE:
            pt1=strstr(pstr,"OpenOffice.org");
            pt1+=15;
            pt2=strchr(pt1,' ');
            pt2[0]='\0';
            strcpy(qstr,pt1);
            break;
/////////////////////////////////// where can be extended            
         }
}

int CSoftInfo::BashShellInfo(unsigned int Int_base,PTRecord_SoftInfo pRec_i)
{
    int succ;   
    if (Int_base!=BASHSHELL_BASE)
    {
        printf("Is BashShell to be displayed?Function Error!\n");
	return -1;
    }
    pRec_i->len_info=BUFSIZE;
    Set_SoftCommand(pRec_i,(char *)BASHSHELL_COMMAND_FIRST);
    if ((succ=Get_SoftInfo(pRec_i->pcommand,pRec_i->RecordBuf,&(pRec_i->len_info)))!=0)
    {
        printf("BashShell have not been installed in current Linux system!\n");
        return -1;
    }
    strcat(pRec_i->RecordBuf,BASHSHELL_COMMAND_SECOND);
    pRec_i->len_info=BUFSIZE;
    Set_SoftCommand(pRec_i,pRec_i->RecordBuf);
    if ((succ=Get_SoftInfo(pRec_i->pcommand,pRec_i->RecordBuf,&(pRec_i->len_info)))!=0)
    {
        printf("BashShell version found failed,you have executed incorrectly!\n");
        return -1;
    }
    return 0;
}

int CSoftInfo::LinuxVerInfo(unsigned int Int_base,PTRecord_SoftInfo pRec_i)
{
    int succ;
    char *index_str;
//    char tverbuf[VERSIONSIZE];
    if (Int_base!=LINUXVER_BASE)
    {
        printf("Is LinuxVer to be displayed?Function Error!\n");
	return -1;
    }
    pRec_i->len_info=BUFSIZE;
    Set_SoftCommand(pRec_i,(char *)LINUXVER_COMMAND);
    if ((succ=Get_SoftInfo(pRec_i->pcommand,pRec_i->RecordBuf,&(pRec_i->len_info)))!=0)
    {
        printf("Linux version found failed,you have executed incorrectly!\n");
        return -1;
    }
//    index_str=index(pRec_i->RecordBuf,'(');
//    memset(index_str,0,(int)(pRec_i->RecordBuf+BUFSIZE-index_str));
    return 0;
}

int CSoftInfo::GccVerInfo(unsigned int Int_base,PTRecord_SoftInfo pRec_i)
{
    int succ;
//    char tverbuf[VERSIONSIZE];
    if (Int_base!=GCCVER_BASE)
    {
        printf("Is GccVer to be displayed?Function Error!\n");
	return -1;
    }
    pRec_i->len_info=BUFSIZE;
    Set_SoftCommand(pRec_i,(char *)GCCVER_COMMAND_FIRST);
    if ((succ=Get_SoftInfo(pRec_i->pcommand,pRec_i->RecordBuf,&(pRec_i->len_info)))!=0)
    {
        printf("Gcc have not been installed in current Linux system!\n");
        return -1;
    }
    strcat(pRec_i->RecordBuf,GCCVER_COMMAND_SECOND);
    pRec_i->len_info=BUFSIZE;
    Set_SoftCommand(pRec_i,pRec_i->RecordBuf);
    if ((succ=Get_SoftInfo(pRec_i->pcommand,pRec_i->RecordBuf,&(pRec_i->len_info)))!=0)
    {
        printf("Gcc version found failed,you have executed incorrectly!\n");
        return -1;
    }
    return 0;
}

int CSoftInfo::FirefoxInfo(unsigned int Int_base,PTRecord_SoftInfo pRec_i)
{
    int succ;
    char *index_str;
    if (Int_base!=FIREFOX_BASE)
          {
        printf("Is LinuxVer to be displayed?Function Error!\n");
	     return -1;
          }
    pRec_i->len_info=BUFSIZE;
    Set_SoftCommand(pRec_i,(char *)FIREFOX_COMMAND);
    if ((succ=Get_SoftInfo(pRec_i->pcommand,pRec_i->RecordBuf,&(pRec_i->len_info)))!=0)
          {
        printf("Linux version found failed,you have executed incorrectly!\n");
        return -1;
          }
    return 0;
}

int CSoftInfo::OpenOfficeInfo(unsigned int Int_base,PTRecord_SoftInfo pRec_i)
{
    int succ;
    char *index_str="OpenOffice.org 2.3  680m6";
    if (Int_base!=OPENOFFICE_BASE)
          {
        printf("Is OpenOffice Version to be displayed?Function Error!\n");
	     return -1;
          }
    pRec_i->len_info=25;
    Set_SoftCommand(pRec_i,(char *)OPENOFFICE_COMMAND);
    strcpy(pRec_i->RecordBuf,index_str);
    return 0;
}
/////////////////////////////////////////////////////////////  
//                                                         //
// where can be extended but individual function be added  //

/////////////////////////////////////////////////////////////

int CSoftInfo::SoftWareInfo(unsigned int base,PTRecord_SoftInfo pprecord)
{
    int result=0;
    char tverbuf[VERSIONSIZE];
    switch(base)
         {
       case BASHSHELL_BASE:
	      result=BashShellInfo(base,pprecord);
	      break;
       case LINUXVER_BASE:
         result=LinuxVerInfo(base,pprecord);      
	      break;
       case GCCVER_BASE:
	      result=GccVerInfo(base,pprecord);
	      break;
       case FIREFOX_BASE:
	      result=FirefoxInfo(base,pprecord);
	      break;
       case OPENOFFICE_BASE:
	      result=OpenOfficeInfo(base,pprecord);
	      break;
/////////////////////////////////// where can be extended	      
       default:
	      return -1;
          }
    version_shortcut(base,pprecord->RecordBuf,tverbuf);
    memset(pprecord->RecordBuf,0,BUFSIZE);
    strcpy(pprecord->RecordBuf,tverbuf);
    return result;
}

void CSoftInfo::write_hash_MainSoftInfo(PTMainSoftInfo pmsoftinfo,char *in_name,char *in_ver)
{
   PTMainSoftInfo pstruct=pmsoftinfo;
   char nameverbuf[256];
   char *md5_str = NULL;
   
   memset((PTMainSoftInfo)pstruct,0,sizeof(TMainSoftInfo));
   memset(nameverbuf,0,256);
   strcpy(pstruct->softname,in_name);
   strcpy(pstruct->version,in_ver);
   strcpy(nameverbuf,in_name);
   strcat(nameverbuf,in_ver);
   md5_str = (char*)MDString(nameverbuf);
   strcpy(pstruct->md5_hash, md5_str);
}

int CSoftInfo::Get_SoftRecord(unsigned int int_softname)
{
   PTRecord_SoftInfo ptrsi,qtrsi;
   ptrsi=qtrsi=p_recordsoftinfo;
   int suc;
   char P_softname[SOFTNAMESIZE];

   Softname_swap(P_softname,int_softname);
   while (ptrsi!=NULL)
       {  
       if (ptrsi->softID==int_softname)
           break;
       qtrsi=ptrsi;
       ptrsi=ptrsi->next;
       }
   if (ptrsi==NULL)
       {
      ptrsi=(PTRecord_SoftInfo)malloc(sizeof(TRecord_SoftInfo));    
      memset(ptrsi,0,sizeof(TRecord_SoftInfo));
      ptrsi->softID=int_softname;
      strcpy(ptrsi->psoftname,P_softname);
      if((suc=SoftWareInfo(int_softname,ptrsi))!=0)
               {
          free(ptrsi);
          printf("No correct parameter has been transited!\n");
          return -1;
               }
      write_hash_MainSoftInfo(&(p_softsave->mainsoftinfo),P_softname,ptrsi->RecordBuf);
      ptrsi->mainsoftinfo=p_softsave->mainsoftinfo;
      if (qtrsi!=NULL)
         qtrsi->next=ptrsi; 
      else 
         p_recordsoftinfo=ptrsi;
        }
   else
       {
       memset(&(p_softsave->mainsoftinfo),0,sizeof(TMainSoftInfo));
       p_softsave->mainsoftinfo=ptrsi->mainsoftinfo;
        }
   return 0;
}

void CSoftInfo::SwitchPrint(unsigned int funcname_ID)
{
   char Pp_softname[SOFTNAMESIZE];
   Softname_swap(Pp_softname,funcname_ID);
   p_softsave->Isorno_success=Get_SoftRecord(funcname_ID);
   if (p_softsave->Isorno_success!=0) 
       printf("Failed while acquiring %s Version information!Please inspect this program and try again!\n",Pp_softname);  
   else
   {
       printf("Success while acquiring %s Version information!\n",Pp_softname);
       printf("Version information of %s is： ",Pp_softname);
       fputs((p_softsave->mainsoftinfo).version,stdout);
       printf("\n");
   }
}

void CSoftInfo::SerializeSoftInfo(IN PTMainSoftInfo pmainsoftinfo, OUT char *out_str, OUT int *out_str_len)
{
    if(out_str == NULL)
        return;

    char *ptr = out_str;
    *out_str_len = 0;
    int n = 0;
    memset(out_str, 0, sizeof(TMainSoftInfo));

    strcpy(ptr, pmainsoftinfo->softname);
    n = strlen(pmainsoftinfo->softname);
    ptr += 1 + n;
    *out_str_len += 1 + n;

    strcpy(ptr, pmainsoftinfo->version);
    n = strlen(pmainsoftinfo->version);
    ptr += 1 + n;
    *out_str_len += 1 + n;

    strcpy(ptr, pmainsoftinfo->md5_hash);
    n = strlen(pmainsoftinfo->md5_hash);
    ptr += 1 + n;
    *out_str_len += 1 + n;

    return;
}

void CSoftInfo::UnserializeSoftStruct(OUT PTMainSoftInfo pmainsoftinfo, IN char *in_str)
{
    if(pmainsoftinfo == NULL || in_str  == NULL)
        return;
    
    memset(pmainsoftinfo, 0, sizeof(TMainSoftInfo));

    char *ptr = in_str;

//    strcpy((*pmainsoftinfo)->softname, ptr);
      strcpy(pmainsoftinfo->softname, ptr);
    ptr += 1 + strlen(pmainsoftinfo->softname);
//    strcpy((*pmainsoftinfo)->version, ptr);
      strcpy(pmainsoftinfo->version, ptr);
    ptr += 1 + strlen(pmainsoftinfo->version);
//    strcpy((*pmainsoftinfo)->md5_hash, ptr);
      strcpy(pmainsoftinfo->md5_hash, ptr);

    return;
}
/*
void CSoftInfo::MultiSwitchPrint(unsigned int multifuncname_ID)
{
    unsigned int temp=((multifuncname_ID^BASHSHELL_BASE)<<15);
    if (((multifuncname_ID^BASHSHELL_BASE)<<15)==0)
       SwitchPrint(BASHSHELL_BASE);
    if (((multifuncname_ID^LINUXVER_BASE)<<14)==0)
       SwitchPrint(LINUXVER_BASE);
    if (((multifuncname_ID^GCCVER_BASE)<<13)==0)
       SwitchPrint(GCCVER_BASE);
    if (((multifuncname_ID^FIREFOX_BASE)<<12)==0)
       SwitchPrint(GCCVER_BASE);
}*/
/////////////////////////////////// where can be extended      


