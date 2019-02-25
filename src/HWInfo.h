#ifndef HWINFO_H
#define HWINFO_H

#include "Defs.h"
/***************************************************************************
 *   Hard Info Macro & Constant Definitions   *
 *      *
 *                                                                         *
 ***************************************************************************/
#define HWI_BUFFER_SIZE 512
#define HW_STRUCT_SERIALIZE_SIZE 4096

//Hardware information print type
#define HWPRT_OTHR		0
#define HWPRT_CPU			1
#define HWPRT_MEM			2
#define HWPRT_HDD			3
#define HWPRT_USB			4
#define HWPRT_NIC			5
#define HWPRT_MTHRBRD		6
#define HWPRT_BIOS			7
#define HWPRT_PC			8

////////////////////Global Variables///////////////////
extern void *__HWLIST__[];

//Hardware information function returning type
typedef int HWINFO_STATUS;
//Hardware information function returning values
#define HWINFO_SUCCESS								0
#define HWINFO_GET_HW_INFO_FAILED			1
#define HWINFO_CHECK_HW_INFO_FAILED		2

////[Help Funcion]////
//Function: Print hardware info. For testing
//Parameters:
//1.print type
//Retruns: void
//Commet: This function can only be called by other functions
inline void helpfunc_hwiprint(int print_type);

void InitHwInfo();

HWINFO_STATUS GetHwInfo();

HWINFO_STATUS FreeHwInfo();

#endif
