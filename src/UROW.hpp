/* 
 * File:   UROW.hpp
 * Author: jjdengjj
 *
 * Created on December 8, 2009, 9:07 PM
 */

#include <mysql++.h>
#include <ssqls.h>

sql_create_3 (UROW, 1, 3,
        mysqlpp::sql_int, ID,
        mysqlpp::sql_varchar, USERNAME,
        mysqlpp::sql_varchar, PASSWD
)
