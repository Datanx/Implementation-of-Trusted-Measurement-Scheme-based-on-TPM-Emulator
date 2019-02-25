#ifndef		IN
#define		IN
#endif

#ifndef		OUT
#define		OUT
#endif

#ifndef		HWCHECK_H
#define		HWCHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include "HWInfo.h"
#include "cpuinfo.h"
#include "usbinfo.h"
#include "hdinfo.h"
#include "nicinfo.h"
#include "dmi_info.h"


typedef int HWCHK_STATUS;
#define		HWCHK_STATUS_SUCCESS	0x0000
#define         HWCHK_STATUS_NO_POLICY_SET  0x0001

#define         HWCHK_STATUS_ERR_CPU    0x0010      //Lack of proper CPU
#define         HWCHK_STATUS_WRONG_CPU    0x0011   //Wrong CPU installed
#define         HWCHK_STATUS_IGNORE_CPU 0x0012

#define         HWCHK_STATUS_ERR_PC     0x0020      //Lack of proper PC information
#define         HWCHK_STATUS_WRONG_PC     0x0021    //Incorrect PC information
#define         HWCHK_STATUS_IGNORE_PC  0x0022

#define         HWCHK_STATUS_ERR_MB     0x0030      //Lack of proper Mother board
#define         HWCHK_STATUS_WRONG_MB   0x0031      //Invalid Mother board installed
#define         HWCHK_STATUS_IGNORE_MB  0x0032

#define         HWCHK_STATUS_ERR_BIOS   0x0040      //Lack of proper BIOS
#define         HWCHK_STATUS_WRONG_BIOS   0x0041    //Invalid BIOS installed
#define         HWCHK_STATUS_IGNORE_BIOS    0x0042

#define         HWCHK_STATUS_ERR_MEM    0x0050      //Lack of proper memory
#define         HWCHK_STATUS_WRONG_MEM    0x0051    //Invalid memory installed
#define         HWCHK_STATUS_IGNORE_MEM     0x0052

#define         HWCHK_STATUS_ERR_HDD    0x0060      //Lack of proper harddisk
#define         HWCHK_STATUS_WRONG_HDD    0x0061    //Invalid harddisk installed
#define         HWCHK_STATUS_IGNORE_HDD     0x0062

#define         HWCHK_STATUS_ERR_NIC    0x0070      //Lack of proper NIC
#define         HWCHK_STATUS_WRONG_NIC    0x0071    //Invalid NIC installed
#define         HWCHK_STATUS_IGNORE_NIC     0x0072

#define         HWCHK_STATUS_ERR_USB    0x0080      //Lack of proper USB device
#define         HWCHK_STATUS_WRONG_USB    0x0081    //Invalid USB device pluginned
#define         HWCHK_STATUS_IGNORE_USB     0x0082

#define		HWCHK_STATUS_FAILED		0x0100

//Get the file name by given keyword
static char* get_filename_by_kw(char* path, char* kw);

//Check CPU Info(Compare with cpuid file in /root/hw)
HWCHK_STATUS CheckCPU(char *cpuinfo_file_path);

//Check USB Info(Compare with usbid file in /root/hw)
HWCHK_STATUS CheckUSB(char *usbinfo_file_path);
int usb_info_check(void *d1, void *d2);

//Check HDD Info(Compare with hddid file in /root/hw)
HWCHK_STATUS CheckHDD(char *hddinfo_file_path);

//Check NIC Info(Compare with hddid file in /root/hw)
HWCHK_STATUS CheckNIC(char *nicinfo_file_path);
int nic_info_check(void *d1, void *d2);

//Check BIOS Info(Compare with hddid file in /root/hw)
HWCHK_STATUS CheckBIOS(char *biosinfo_file_path);

//Check Mother board Info(Compare with hddid file in /root/hw)
HWCHK_STATUS CheckMB(char *mbinfo_file_path);

//Check Memory Info(Compare with hddid file in /root/hw)
HWCHK_STATUS CheckMem(char *meminfo_file_path);
int mem_info_check(void *d1, void *d2);

//Check PC Info(Compare with hddid file in /root/hw)
HWCHK_STATUS CheckPC(char *pcinfo_file_path);

#endif
