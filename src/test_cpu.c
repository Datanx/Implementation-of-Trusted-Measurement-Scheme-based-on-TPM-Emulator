#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpuinfo.h"

int main()
{
	CPU_INFO cpu;
	
	InitCPUStruct(&cpu);
	FillCPUStruct(&cpu);
	print_cpu_info(&cpu);
		
	return 0;
}
