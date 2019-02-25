/* 
 * File:   SPROW.hpp
 * Author: jjdengjj
 *
 * Created on December 7, 2009, 9:28 PM
 */

#include <mysql++.h>
#include <ssqls.h>

sql_create_4 (SPROW, 1, 4,
        mysqlpp::sql_int, ID,
        mysqlpp::sql_varchar, SERVICENAME,
        mysqlpp::sql_varchar, HPOLICY,
        mysqlpp::sql_varchar, SPOLICY
)

