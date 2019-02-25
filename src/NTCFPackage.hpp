/* 
 * File:   NCSFPackage.hpp
 * Author: jjdengjj
 *
 * Created on November 24, 2009, 2:55 PM
 */

#ifndef _NCSFPACKAGE_HPP
#define	_NCSFPACKAGE_HPP

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "NTCFPACKAGECODE.hpp"
extern "C"
{
#include "base64.h"
}


class NTCFPackage {
public:
    NTCFPackage();
    NTCFPackage(const NTCFPackage& orig);
    virtual ~NTCFPackage();
private:
    int PACKAGE_MAJOR_CODE;
    int PACKAGE_MINOR_CODE;
    char* PACKAGE_DATA_INFO;
    char* PACKAGE_ETRA_INFO;
    char* PACKAGE_SERIALIZE_STR;
    int PACKAGE_SERIALIZE_STRLEN;
    char* PACKAGE_BASE64_STR;

public:
    //Getter
    int get_MajorCode();
    int get_MinorCode();
    void get_DataInfo(char**);
    void get_EtraInfo(char**);
    void get_SerializeStr(char**);
    int get_SerializeStrLen();
    void get_Base64Str(char**);
    
    //Setter
    void set_MajorCode(const int);
    void set_MinorCode(const int);
    void set_DataInfo(const char*);
    void set_EtraInfo(const char*);
    void set_SerializeStr(const char*, const int);
    void set_Base64Str(const char*);

    //Operator
    NTCFPackage& operator=(const NTCFPackage&);

private:
    void Serialize();
    void UnSerialize();
    void Base64();
    void UnBase64();

    static int HexStrToInt(char*);
    static int HexPow(int);
    static int CharToInt(char);
};

#endif	/* _NCSFPACKAGE_HPP */

