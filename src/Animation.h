#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef ANIMATION_H
#define ANIMATION_H
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CH1 "-"
#define CH2 "\\"
#define CH3 "|"
#define CH4 "/"
#define BAK "\b"
#define SOW_ON 1
#define SOW_OFF 0

void RotatePrint(IN unsigned int, IN char*, IN int);
int IntToLen(IN unsigned int);

#endif
