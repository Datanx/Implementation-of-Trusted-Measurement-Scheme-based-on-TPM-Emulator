#ifndef		IN
#define		IN
#endif

#ifndef		OUT
#define		OUT
#endif

#ifndef	HDINFO_H
#define HDINFO_H

//Harddisk info structure
typedef struct _HDD_INFO
{
	char Model[64];
	char SN[64];
	char md5_hash[40];
} HDD_INFO, *PHDD_INFO;

////[Normal Function]////
//Function: Initialize HDD info structure
//Parameters:
//Param 1: PHDD_INFO pointer
//Returns: void
void InitHDDStruct(OUT PHDD_INFO *hddinfo);

////[Normal Function]////
//Function: Filling the HDD info structure
//Parameters:
//Param 1: PHDD_INFO variable
//Returns: void
void FillHDDStruct(IN PHDD_INFO hddinfo);

////[Normal Function]////
//Function: Free HDD info structure
//Parameters:
//Param 1: PHDD_INFO variable
//Returns: void
void FreeHDDStruct(IN PHDD_INFO hddinfo);

void SerializeHDDStruct(IN PHDD_INFO hddinfo, OUT char *out_str, OUT int *out_str_len);

void UnserializeHDDStruct(OUT PHDD_INFO *hddinfo, IN char *in_str);

////[help Function]////
//Function: Print HDD info structure
//Parameters:
//Param 1: PHDD_INFO variable
//Returns: void
//Comment: this function only for test
void print_hdd_info(IN PHDD_INFO hddstruct);
#endif
