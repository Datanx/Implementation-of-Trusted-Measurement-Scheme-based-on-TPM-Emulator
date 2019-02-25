#include "alg.h"

int BinSrch(IN void *array, IN void *data, IN int spivot, IN int epivot, IN BS_CmpFunc cmpfunc)
{
	int i = (spivot + epivot) / 2;
	if(spivot == i)
		return -1;
	int cmpresult = 0;
	
	cmpresult = cmpfunc(array,i, data);
	
	if(cmpresult < 0)		//param1 < param2
		return BinSrch(array, data, i, epivot, cmpfunc);
	else if(cmpresult > 0)	//param1 > param2
		return BinSrch(array, data, spivot, i, cmpfunc);
	else if(cmpresult == 0)	//param1 = param2
		return i;
}
