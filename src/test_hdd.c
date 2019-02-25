#include <stdio.h>
#include <stdlib.h>
#include "hdinfo.h"

int main()
{
	PHDD_INFO phdd = NULL;
	InitHDDStruct(&phdd);
	FillHDDStruct(phdd);
	print_hdd_info(phdd);
	FreeHDDStruct(phdd);
}
