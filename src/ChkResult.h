#ifndef		IN
#define		IN
#endif

#ifndef		OUT
#define		OUT
#endif

#ifndef		CHKRESULT_H
#define		CHKRESULT_H
/////////////Definition of checking result type////////////
//if the value of one bit is 1, the corresponding hardware or software has been checked successfully, otherwise, checkint failed.
typedef unsigned short CHK_RESULT;		//2 bytes
//////The meaning of each bit of hardware checking policy type (from right to left bit)//////
//bit 0: Other device
//bit 1: CPU
//bit 2: Memory
//bit 3: Hard disk
//bit 4: USB device
//bit 5: Network interface card
//bit 6: Mother board
//bit 7: BIOS
//bit 8: PC
//bit 9 ~ bit 15: Undefined

//////The meaning of each bit of software checking result type(from right to left bit)//////
//bit 0: Other software
//bit 1: Kernel
//bit 2: bash
//bit 3: gcc
//bit 4: openoffice
//bit 5: firefox
//bit 6 ~ bit 15: Undefined

////////Check result value set macro/////////
#define CHKRES_SET_ON(__chkres__, __restype__) \
				__chkres__ |= 0x0001 << __restype__

#define CHKRES_SET_OFF(__chkres__, __restype__) \
				__chkres__ &= ~(0x0001 << __restype__)
				
#define CHKRES_GET(__chkres__, __restype__) \
				(__chkres__ >> __restype__) & 0x0001

#endif
