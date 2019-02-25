/* 
 * File:   CPROW.hpp
 * Author: jjdengjj
 *
 * Created on December 6, 2009, 2:59 PM
 */
#include <mysql++.h>
#include <ssqls.h>

sql_create_11 (CPROW, 1, 11,
        mysqlpp::sql_int, ID,
        mysqlpp::sql_varchar, PCSN,
        mysqlpp::sql_varchar, MBSN,
        mysqlpp::sql_varchar, BIOSSN,
        mysqlpp::sql_varchar, CPUSN,
        mysqlpp::sql_varchar, HDDSN,
        mysqlpp::sql_varchar, MEMSN,
        mysqlpp::sql_varchar, NICSN,
        mysqlpp::sql_varchar, USBSN,
        mysqlpp::sql_varchar, HPOLICY,
        mysqlpp::sql_varchar, SPOLICY
)




