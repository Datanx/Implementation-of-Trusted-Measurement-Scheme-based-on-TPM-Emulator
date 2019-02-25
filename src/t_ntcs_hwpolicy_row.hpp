#include <mysql++.h>
#include <ssqls.h>

sql_create_22 (t_ntcs_hwpolicy_row, 1, 22,
        mysqlpp::sql_int,     np_autoId,
        mysqlpp::sql_varchar, np_domainName,
        mysqlpp::sql_varchar, np_machineId,
        mysqlpp::sql_varchar, np_serviceName,
        mysqlpp::sql_varchar, np_policyStr,
        mysqlpp::sql_varchar, np_identStr,
        mysqlpp::sql_varchar, np_swIdentStr,
        mysqlpp::sql_varchar, np_hashPc,
        mysqlpp::sql_varchar, np_hashBios,
        mysqlpp::sql_varchar, np_hashMb,
        mysqlpp::sql_varchar, np_hashCpu,
        mysqlpp::sql_text,    np_hashMem,
        mysqlpp::sql_varchar, np_hashHdd,
        mysqlpp::sql_text,    np_hashUsb,
        mysqlpp::sql_text,    np_hashNic,
        mysqlpp::sql_varchar, np_Reserve1,
        mysqlpp::sql_varchar, np_Reserve2,
        mysqlpp::sql_varchar, np_Reserve3,
        mysqlpp::sql_varchar, np_Reserve4,
        mysqlpp::sql_varchar, np_Reserve5,
        mysqlpp::sql_varchar, np_Reserve6,
        mysqlpp::sql_text,    np_addition
);
