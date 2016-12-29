#ifndef __SC_MAPTABLE_H__
#define __SC_MAPTABLE_H__

#include "SC_Protocol.h"
#ifdef __cplusplus
extern"C" {
#endif
/************************************************************************/
enum sc_tag_id
{
	    SC_TAG_MSK   = 0xFFFFFF,
	    SC_TAG_NULL  = 0x7FFFFFFF,
};

#define SC_TAG_INT(str)    str//(((str[2] << 16) | (str[1] << 8) | str[0]))
#define SC_SUBTAG_INT(str)  str//(((str[2] << 16) | (str[1] << 8) | str[0]))

	
extern const sc_maintagmap_st SC_MainTagMapTab[];
/************************************************************************/
#ifdef __cplusplus
}
#endif
#endif //__SC_MAPTABLE_H__
