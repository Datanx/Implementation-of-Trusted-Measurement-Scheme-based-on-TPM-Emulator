#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pcihdr.h"
#include "alg.h"

int bs_cmpfun(void *array, int index, void *d)
{
	PPCI_VENTABLE pci_array = (PPCI_VENTABLE)array;
	PPCI_VENTABLE elem = &(pci_array[index]);
	int data = *((int *)d);
	
	if(elem->VenId < data)
		return -1;
	else if(elem->VenId > data)
		return 1;
	else
		return 0;
}

int main()
{
	int srchdata = 0x10EC;
	int srchresult = BinSrch(PciVenTable, &srchdata, 0, PCI_VENTABLE_LEN-1, bs_cmpfun);
	
	printf("Match Index=%d\n", srchresult);
	printf("Vendor Info: %s\n", PciVenTable[srchresult].VenFull);

	return 0;
}
