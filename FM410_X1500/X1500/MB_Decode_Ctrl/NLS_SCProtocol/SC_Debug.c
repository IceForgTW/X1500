//#include <stdio.h>
//#include <string.h>
#include <stdarg.h>
#include "SC_Debug.h"

void SC_Debug(char* Format,...)
{

    va_list	      vParms;
    unsigned int  nLen = 0;
    char          chBuf[SC_DEBUG_BUF_LEN];

	memset(chBuf, 0, SC_DEBUG_BUF_LEN);
	va_start(vParms, Format);
	nLen += vsprintf(chBuf, Format, vParms);
	va_end(vParms);
    
#ifndef SC_DEBUG
	//API_SendComm(chBuf, nLen);
	Y_commWrite(chBuf, nLen);
#endif
}

