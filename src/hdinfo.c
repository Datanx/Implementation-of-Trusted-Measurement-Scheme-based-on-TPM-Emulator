#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>
#include <sys/fcntl.h>
#include "lib_StringAdd.h"
#include "hdinfo.h"
#include "HWInfo.h"

void InitHDDStruct(OUT PHDD_INFO *hddinfo)
{
	if(*hddinfo != NULL)
		free(*hddinfo);
	
	*hddinfo = NULL;
	*hddinfo = (PHDD_INFO)malloc(sizeof(HDD_INFO));
	
	if(*hddinfo != NULL)
	{
		memset((*hddinfo)->Model, 0, 64);
		memset((*hddinfo)->SN, 0, 64);
		memset((*hddinfo)->md5_hash, 0, 40);
	}
}

void FillHDDStruct(IN PHDD_INFO hddinfo)
{
	struct hd_driveid id;
	char model[64]={0};
	char sn[64]={0};
	char infobuffer[256]={0};
	char *md5_str = NULL;
	char *chptr = NULL;
	int n=0;
	
	int fd = open("/dev/hda", O_RDONLY|O_NONBLOCK);
	if (fd < 0)
	{
		//try to open /dev/hda
		fd = open("/dev/sda", O_RDONLY|O_NONBLOCK);
		if(fd < 0)
		{
			perror("detecting harddisk");
			return;
		}
	}
	if(!ioctl(fd, HDIO_GET_IDENTITY, &id))
	{
		//Build HDD Model
		RTrim(id.model, 40, model);
		n = strlen(model);
		strcpy(hddinfo->Model, model);
		strcpy(infobuffer, model);
		chptr = infobuffer + n;
		
		//Build SerialNumber
		LTrim(id.serial_no, sn);
		n = strlen(sn);
		strcpy(hddinfo->SN, sn);
		strcpy(chptr, sn);
		chptr += n;
		
		//Calculate md5 hash
		md5_str = (char*)MDString(infobuffer);
		strcpy(hddinfo->md5_hash, md5_str);
	}
}

void FreeHDDStruct(IN PHDD_INFO hddinfo)
{
	if(hddinfo != NULL)
		free(hddinfo);
}

void SerializeHDDStruct(IN PHDD_INFO hddinfo, OUT char *out_str, OUT int *out_str_len)
{
     if(out_str == NULL)
        return;

    char *ptr = out_str;
    *out_str_len = 0;
    int n = 0;
    memset(out_str, 0, HW_STRUCT_SERIALIZE_SIZE);

    strcpy(ptr, hddinfo->Model);
    n = strlen(hddinfo->Model);
    ptr += 1 + n;
    *out_str_len += 1 + n;

    strcpy(ptr, hddinfo->SN);
    n = strlen(hddinfo->SN);
    ptr += 1 + n;
    *out_str_len += 1 + n;

    strcpy(ptr, hddinfo->md5_hash);
    n = strlen(hddinfo->md5_hash);
    ptr += 1 + n;
    *out_str_len += 1 + n;

    return;
}

void UnserializeHDDStruct(OUT PHDD_INFO *hddinfo, IN char *in_str)
{
    if(hddinfo == NULL || in_str  == NULL)
        return;

    char *ptr = in_str;

    strcpy((*hddinfo)->Model, ptr);
    ptr += 1 + strlen((*hddinfo)->Model);
    strcpy((*hddinfo)->SN, ptr);
    ptr += 1 + strlen((*hddinfo)->SN);
    strcpy((*hddinfo)->md5_hash, ptr);

    return;
}

void print_hdd_info(IN PHDD_INFO hddstruct)
{
	if(hddstruct == NULL)
		return;
	
	printf("HDD Information:\n");
	printf("\tHDD Model: %s\n", hddstruct->Model);
	printf("\tHDD Serial Number: %s\n", hddstruct->SN);
	printf("\tHDD Info hash value: %s\n", hddstruct->md5_hash);
}
