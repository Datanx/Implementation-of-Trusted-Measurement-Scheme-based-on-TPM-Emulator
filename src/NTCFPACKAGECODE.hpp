/* 
 * File:   NCSFPACKAGECODE.hpp
 * Author: jjdengjj
 *
 * Created on November 25, 2009, 3:36 PM
 */

#ifndef _NCSFPACKAGECODE_HPP
#define	_NCSFPACKAGECODE_HPP


//Major Code for Command and Communication
#define _NCSFPC_MJ_CMDCOM_NUL_     0x00
#define _NCSFPC_MJ_CMD_REQ_     0x11
#define _NCSFPC_MJ_CMD_RTN_     0x12
#define _NCSFPC_MJ_COM_CLS_     0x21

//Minor Code for REQ and RTN
#define _NCSFPC_MN_REQRTN_NUL_          0x00
#define _NCSFPC_MN_TYPE_HTTP_          0x10
#define _NCSFPC_MN_TYPE_MULTICAST_ 0x20

//RTN Data
#define _NCSFPC_RTN_NON_    "NULL"
#define _NCSFPC_RTN_CLOSE_  "CLOSE"
#define _NCSFPC_RTN_FAILED_  "FAILED"
#define _NCSFPC_RTN_SUCCESS_ "SUCCESS"

#endif	/* _NCSFPACKAGECODE_HPP */

