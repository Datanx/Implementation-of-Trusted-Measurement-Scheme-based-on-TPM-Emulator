/* 
 * File:   NCSFPackage.cpp
 * Author: jjdengjj
 * 
 * Created on November 24, 2009, 2:55 PM
 */

#include "NTCFPackage.hpp"

NTCFPackage::NTCFPackage() {
    this->PACKAGE_MAJOR_CODE = _NCSFPC_MJ_CMDCOM_NUL_;
    this->PACKAGE_MINOR_CODE = _NCSFPC_MN_REQRTN_NUL_;
    this->PACKAGE_DATA_INFO = NULL;
    this->PACKAGE_ETRA_INFO = NULL;
    this->PACKAGE_SERIALIZE_STR = NULL;
    this->PACKAGE_SERIALIZE_STRLEN = 0;
    this->PACKAGE_BASE64_STR = NULL;
}

NTCFPackage::NTCFPackage(const NTCFPackage& orig) {
    this->PACKAGE_MAJOR_CODE = orig.PACKAGE_MAJOR_CODE;
    this->PACKAGE_MINOR_CODE = orig.PACKAGE_MINOR_CODE;

    if(orig.PACKAGE_DATA_INFO != NULL)
    {
        this->PACKAGE_DATA_INFO = (char*)malloc((strlen(orig.PACKAGE_DATA_INFO)+1)*sizeof(char));
        bzero(this->PACKAGE_DATA_INFO, (strlen(orig.PACKAGE_DATA_INFO)+1)*sizeof(char));
        memcpy(this->PACKAGE_DATA_INFO, orig.PACKAGE_DATA_INFO, strlen(orig.PACKAGE_DATA_INFO)*sizeof(char));
    }
    else
        this->PACKAGE_DATA_INFO = NULL;

    if(orig.PACKAGE_ETRA_INFO != NULL)
    {
        this->PACKAGE_ETRA_INFO = (char*)malloc((strlen(orig.PACKAGE_ETRA_INFO)+1)*sizeof(char));
        bzero(this->PACKAGE_ETRA_INFO, (strlen(orig.PACKAGE_ETRA_INFO)+1)*sizeof(char));
        memcpy(this->PACKAGE_ETRA_INFO, orig.PACKAGE_ETRA_INFO, strlen(orig.PACKAGE_ETRA_INFO)*sizeof(char));
    }
    else
        this->PACKAGE_ETRA_INFO = NULL;

    this->PACKAGE_SERIALIZE_STR = NULL;
    this->PACKAGE_SERIALIZE_STRLEN = 0;
    this->PACKAGE_BASE64_STR = NULL;

    this->Serialize();
    this->Base64();
}

NTCFPackage::~NTCFPackage() {
    if(this->PACKAGE_DATA_INFO!=NULL)
        free(this->PACKAGE_DATA_INFO);
    if(this->PACKAGE_ETRA_INFO!=NULL)
        free(this->PACKAGE_ETRA_INFO);
    if(this->PACKAGE_SERIALIZE_STR!=NULL)
        free(this->PACKAGE_SERIALIZE_STR);
    if(this->PACKAGE_BASE64_STR!=NULL)
        free(this->PACKAGE_BASE64_STR);
}

void NTCFPackage::set_MajorCode(const int mjc)
{
    this->PACKAGE_MAJOR_CODE = mjc;
    this->Serialize();
    this->Base64();
}

int NTCFPackage::get_MajorCode()
{
    return this->PACKAGE_MAJOR_CODE;
}

void NTCFPackage::set_MinorCode(const int mic)
{
    this->PACKAGE_MINOR_CODE = mic;
    this->Serialize();
    this->Base64();
}

int NTCFPackage::get_MinorCode()
{
    return this->PACKAGE_MINOR_CODE;
}

void NTCFPackage::set_DataInfo(const char* di)
{
    if(this->PACKAGE_DATA_INFO != NULL)
        free(this->PACKAGE_DATA_INFO);

    if(di == NULL)
        this->PACKAGE_DATA_INFO = NULL;
    else
    {
        this->PACKAGE_DATA_INFO = (char*)malloc((strlen(di)+1)*sizeof(char));
        bzero(this->PACKAGE_DATA_INFO, (strlen(di)+1)*sizeof(char));
        memcpy(this->PACKAGE_DATA_INFO, di, strlen(di));
    }

    this->Serialize();
    this->Base64();
}

void NTCFPackage::get_DataInfo(char** rtn)
{
    if(this->PACKAGE_DATA_INFO == NULL)
        *rtn = NULL;

    *rtn = (char*)malloc((strlen(this->PACKAGE_DATA_INFO)+1)*sizeof(char));
    bzero(*rtn, (strlen(this->PACKAGE_DATA_INFO)+1)*sizeof(char));
    memcpy(*rtn, this->PACKAGE_DATA_INFO, strlen(this->PACKAGE_DATA_INFO)*sizeof(char));
}

void NTCFPackage::set_EtraInfo(const char* ei)
{
    if(this->PACKAGE_ETRA_INFO != NULL)
        free(this->PACKAGE_ETRA_INFO);

    if(ei == NULL)
        this->PACKAGE_ETRA_INFO = NULL;
    else
    {
        this->PACKAGE_ETRA_INFO = (char*)malloc((strlen(ei)+1)*sizeof(char));
        bzero(this->PACKAGE_ETRA_INFO, (strlen(ei)+1)*sizeof(char));
        memcpy(this->PACKAGE_ETRA_INFO, ei, strlen(ei)*sizeof(char));
    }
    this->Serialize();
    this->Base64();
}

void NTCFPackage::get_EtraInfo(char** rtn)
{
    if(this->PACKAGE_ETRA_INFO == NULL)
        *rtn = NULL;

    *rtn = (char*)malloc((strlen(this->PACKAGE_ETRA_INFO)+1)*sizeof(char));
    bzero(*rtn, (strlen(this->PACKAGE_ETRA_INFO)+1)*sizeof(char));
    memcpy(*rtn, this->PACKAGE_ETRA_INFO, strlen(this->PACKAGE_ETRA_INFO)*sizeof(char));
}

void NTCFPackage::set_SerializeStr(const char* ss, const int len)
{
    if(this->PACKAGE_SERIALIZE_STR != NULL)
        free(this->PACKAGE_SERIALIZE_STR);

    if(ss == NULL)
    {
        this->PACKAGE_SERIALIZE_STR = NULL;
        this->PACKAGE_SERIALIZE_STRLEN = 0;
    }
    else
    {
        this->PACKAGE_SERIALIZE_STR = (char*)malloc((len+1)*sizeof(char));
        bzero(this->PACKAGE_SERIALIZE_STR, (len+1)*sizeof(char));
        memcpy(this->PACKAGE_SERIALIZE_STR, ss, len*sizeof(char));
        this->PACKAGE_SERIALIZE_STRLEN = len;
    }
    this->UnSerialize();
    this->Base64();
}

void NTCFPackage::get_SerializeStr(char** rtn)
{
    if(this->PACKAGE_SERIALIZE_STR == NULL)
        *rtn = NULL;

    *rtn = (char*)malloc((this->PACKAGE_SERIALIZE_STRLEN+1)*sizeof(char));
    bzero(*rtn, (this->PACKAGE_SERIALIZE_STRLEN+1)*sizeof(char));
    memcpy(*rtn, this->PACKAGE_SERIALIZE_STR, this->PACKAGE_SERIALIZE_STRLEN*sizeof(char));
}

int NTCFPackage::get_SerializeStrLen()
{
    return this->PACKAGE_SERIALIZE_STRLEN;
}

void NTCFPackage::set_Base64Str(const char* bs)
{
    if(this->PACKAGE_BASE64_STR != NULL)
        free(this->PACKAGE_BASE64_STR);

    if(bs == NULL)
        this->PACKAGE_BASE64_STR = NULL;
    else
    {
        this->PACKAGE_BASE64_STR = (char*)malloc((strlen(bs)+1)*sizeof(char));
        bzero(this->PACKAGE_BASE64_STR, (strlen(bs)+1)*sizeof(char));
        memcpy(this->PACKAGE_BASE64_STR, bs, strlen(bs)*sizeof(char));
    }
    this->UnBase64();
    this->UnSerialize();
}

void NTCFPackage::get_Base64Str(char** rtn)
{
     if(this->PACKAGE_BASE64_STR == NULL)
        *rtn = NULL;

    *rtn = (char*)malloc((strlen(this->PACKAGE_BASE64_STR)+1)*sizeof(char));
    bzero(*rtn, (strlen(this->PACKAGE_BASE64_STR)+1)*sizeof(char));
    memcpy(*rtn, this->PACKAGE_BASE64_STR, strlen(this->PACKAGE_BASE64_STR)*sizeof(char));
}

NTCFPackage& NTCFPackage::operator =(const NTCFPackage& orig)
{
    this->PACKAGE_MAJOR_CODE = orig.PACKAGE_MAJOR_CODE;
    this->PACKAGE_MINOR_CODE = orig.PACKAGE_MINOR_CODE;

    if(this->PACKAGE_DATA_INFO != NULL)
    {
        free(this->PACKAGE_DATA_INFO);
        this->PACKAGE_DATA_INFO = NULL;
    }
    if(orig.PACKAGE_DATA_INFO != NULL)
    {
        this->PACKAGE_DATA_INFO = (char*)malloc((strlen(orig.PACKAGE_DATA_INFO)+1)*sizeof(char));
        bzero(this->PACKAGE_DATA_INFO, (strlen(orig.PACKAGE_DATA_INFO)+1)*sizeof(char));
        memcpy(this->PACKAGE_DATA_INFO, orig.PACKAGE_DATA_INFO, strlen(orig.PACKAGE_DATA_INFO)*sizeof(char));
    }

    if(this->PACKAGE_ETRA_INFO != NULL)
    {
        free(this->PACKAGE_ETRA_INFO);
        this->PACKAGE_ETRA_INFO = NULL;
    }
    if(orig.PACKAGE_ETRA_INFO != NULL)
    {
        this->PACKAGE_ETRA_INFO = (char*)malloc((strlen(orig.PACKAGE_ETRA_INFO)+1)*sizeof(char));
        bzero(this->PACKAGE_ETRA_INFO, (strlen(orig.PACKAGE_ETRA_INFO)+1)*sizeof(char));
        memcpy(this->PACKAGE_ETRA_INFO, orig.PACKAGE_ETRA_INFO, strlen(orig.PACKAGE_ETRA_INFO)*sizeof(char));
    }

    if(this->PACKAGE_SERIALIZE_STR != NULL)
    {
        free(this->PACKAGE_SERIALIZE_STR);
        this->PACKAGE_SERIALIZE_STR = NULL;
        this->PACKAGE_SERIALIZE_STRLEN = 0;
    }
    if(this->PACKAGE_BASE64_STR != NULL)
    {
        free(this->PACKAGE_BASE64_STR);
        this->PACKAGE_BASE64_STR = NULL;
    }

    this->Serialize();
    this->Base64();
}

void NTCFPackage::Serialize()
{
    int sizeD = 0;
    int sizeE = 0;
    if(this->PACKAGE_DATA_INFO != NULL)
        sizeD = strlen(this->PACKAGE_DATA_INFO);
    if(this->PACKAGE_ETRA_INFO != NULL)
        sizeE = strlen(this->PACKAGE_ETRA_INFO);

    char* bufS = (char*)malloc(2*sizeof(int) + (4+sizeD+sizeE)*sizeof(char));
    bzero(bufS, 2*sizeof(int) + (4+sizeD+sizeE)*sizeof(char));
    char* tmp = bufS;
    char* tmpMJ = (char*)malloc(sizeof(int) + sizeof(char));
    bzero(tmpMJ, sizeof(int) + sizeof(char));
    char* tmpMN = (char*)malloc(sizeof(int) + sizeof(char));
    bzero(tmpMN, sizeof(int) + sizeof(char));
    sprintf(tmpMJ, "%04X", this->PACKAGE_MAJOR_CODE);
    sprintf(tmpMN, "%04X", this->PACKAGE_MINOR_CODE);
    memcpy(tmp, tmpMJ, sizeof(int));
    free(tmpMJ);
    tmp+= sizeof(int) + sizeof(char);
    memcpy(tmp, tmpMN, sizeof(int));
    free(tmpMN);
    tmp+= sizeof(int) + sizeof(char);
    if(sizeD != 0)
    {
        memcpy(tmp, this->PACKAGE_DATA_INFO, sizeD*sizeof(char));
        tmp+=sizeD + sizeof(char);
    }
    else
        tmp+=sizeof(char);
    if(sizeE !=0)
    {
        memcpy(tmp, this->PACKAGE_ETRA_INFO, sizeE*sizeof(char));
        tmp+=sizeE +sizeof(char);
    }
    else
        tmp+=sizeof(char);

    if(this->PACKAGE_SERIALIZE_STR != NULL)
        free(this->PACKAGE_SERIALIZE_STR);
    
    this->PACKAGE_SERIALIZE_STR = bufS;
    this->PACKAGE_SERIALIZE_STRLEN = 2*sizeof(int) + (3+sizeD+sizeE)*sizeof(char);

}

void NTCFPackage::UnSerialize()
{
    if(this->PACKAGE_SERIALIZE_STR == NULL)
    {
        this->PACKAGE_MAJOR_CODE = _NCSFPC_MJ_CMDCOM_NUL_;
        this->PACKAGE_MINOR_CODE = _NCSFPC_MN_REQRTN_NUL_;
        this->PACKAGE_DATA_INFO = NULL;
        this->PACKAGE_ETRA_INFO = NULL;
        this->PACKAGE_SERIALIZE_STR = NULL;
        this->PACKAGE_SERIALIZE_STRLEN = 0;
        this->PACKAGE_BASE64_STR = NULL;
    }
    else
    {
        char* tmp = this->PACKAGE_SERIALIZE_STR;
        char* MJCode = (char*)malloc((strlen(tmp)+1)*sizeof(char));
        bzero(MJCode, strlen(tmp)+1);
        strcpy(MJCode, tmp);
        this->PACKAGE_MAJOR_CODE = NTCFPackage::HexStrToInt(MJCode);
        free(MJCode);

        tmp += strlen(tmp)+1;
        char* MNCode = (char*)malloc(strlen(tmp)+1);
        bzero(MNCode, (strlen(tmp)+1)*sizeof(char));
        strcpy(MNCode, tmp);
        this->PACKAGE_MINOR_CODE = NTCFPackage::HexStrToInt(MNCode);
        free(MNCode);

        tmp += strlen(tmp)+1;
        if(this->PACKAGE_DATA_INFO != NULL)
            free(this->PACKAGE_DATA_INFO);
        if(strlen(tmp) != 0)
        {
            this->PACKAGE_DATA_INFO = (char*)malloc((strlen(tmp)+1)*sizeof(char));
            bzero(this->PACKAGE_DATA_INFO, (strlen(tmp)+1)*sizeof(char));
            strcpy(this->PACKAGE_DATA_INFO, tmp);
        }
        else
            this->PACKAGE_DATA_INFO = NULL;

        tmp += strlen(tmp)+1;
        if(this->PACKAGE_ETRA_INFO != NULL)
            free(this->PACKAGE_ETRA_INFO);
        if(strlen(tmp) != 0)
        {
            this->PACKAGE_ETRA_INFO = (char*)malloc((strlen(tmp)+1)*sizeof(char));
            bzero(this->PACKAGE_ETRA_INFO, (strlen(tmp)+1)*sizeof(char));
            strcpy(this->PACKAGE_ETRA_INFO, tmp);
        }
        else
            this->PACKAGE_ETRA_INFO = NULL;
        tmp = NULL;
    }
}

void NTCFPackage::Base64()
{
    if(this->PACKAGE_SERIALIZE_STR != NULL)
    {
        char* ch_enbase64 = NULL;
        ch_enbase64 = base64_encode(this->PACKAGE_SERIALIZE_STR, this->PACKAGE_SERIALIZE_STRLEN);
        if(this->PACKAGE_BASE64_STR != NULL)
            free(this->PACKAGE_BASE64_STR);
        this->PACKAGE_BASE64_STR = (char*)malloc((strlen(ch_enbase64)+1)*sizeof(char));
        bzero(this->PACKAGE_BASE64_STR, (strlen(ch_enbase64)+1)*sizeof(char));
        memcpy(this->PACKAGE_BASE64_STR, ch_enbase64, strlen(ch_enbase64)*sizeof(char));
        free(ch_enbase64);
    }
    else
        this->PACKAGE_BASE64_STR = NULL;
}

void NTCFPackage::UnBase64()
{
    if(this->PACKAGE_BASE64_STR != NULL)
    {
        char* ch_debase64 = NULL;
        int len =0;
        ch_debase64 = base64_delencode(this->PACKAGE_BASE64_STR, &len);
        if(this->PACKAGE_SERIALIZE_STR != NULL)
            free(this->PACKAGE_SERIALIZE_STR);
        this->PACKAGE_SERIALIZE_STR = (char*)malloc(len*sizeof(char));
        bzero(this->PACKAGE_SERIALIZE_STR, len*sizeof(char));
        memcpy(this->PACKAGE_SERIALIZE_STR, ch_debase64, (len-1)*sizeof(char));
        free(ch_debase64);
    }
    else
        this->PACKAGE_SERIALIZE_STR = NULL;
}

int NTCFPackage::HexStrToInt(char* s)
{
    int count = strlen(s);
    int value = 0;
    while(*s != '\0')
    {
        value += CharToInt(*s)*HexPow(count-1);
        count--;
        s++;
    }
    return value;
}

int NTCFPackage::HexPow(int n)
{
    int pow = 1;
    for(int i = 0; i < n; i++)
        pow *= 16;
    return pow;
}

int NTCFPackage::CharToInt(char c)
{
    if(c >= '0' && c<= '9')
        return int(c-'0');
    if(c >= 'a' && c<= 'f')
        return int(c-'a'+10);
    if(c >= 'A' && c<= 'F')
        return int(c-'A'+10);
}
