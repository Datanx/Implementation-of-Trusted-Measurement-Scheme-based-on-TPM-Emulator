//Some Algorithms

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef ALG_H
#define ALG_H

//Binary Search function
typedef int(*BS_CmpFunc)(void *array, int index, void *data);		//Compare function type for binary seach
int BinSrch(IN void *array, IN void *data, IN int spivot, IN int epivot, IN BS_CmpFunc cmpfunc);

#endif
