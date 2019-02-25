#include <mysql++.h>
#include <ssqls.h>

sql_create_8 (t_ntcs_swpolicy_row, 1, 8,
	mysqlpp::sql_int,	nswp_autoId,
	mysqlpp::sql_varchar,	nswp_identStr,
	mysqlpp::sql_varchar,	nswp_Kernel,
	mysqlpp::sql_varchar,	nswp_Gcc,
	mysqlpp::sql_varchar,	nswp_Bash,
	/*mysqlpp::sql_varchar,	nswp_sysSwReserve1,
	mysqlpp::sql_varchar,	nswp_sysSwReserve2,
	mysqlpp::sql_varchar,	nswp_sysSwReserve3,
	mysqlpp::sql_varchar,	nswp_sysSwReserve4,
	mysqlpp::sql_varchar,	nswp_sysSwReserve5,
	mysqlpp::sql_varchar,	nswp_sysSwReserve6,
	mysqlpp::sql_varchar,	nswp_sysSwReserve7,*/
	mysqlpp::sql_varchar,	nswp_Openoffice,
	mysqlpp::sql_varchar,	nswp_Firefox,
	/*mysqlpp::sql_varchar,	nswp_appSwReserve1,
	mysqlpp::sql_varchar,	nswp_appSwReserve2,
	mysqlpp::sql_varchar,	nswp_appSwReserve3,
	mysqlpp::sql_varchar,	nswp_appSwReserve4,
	mysqlpp::sql_varchar,	nswp_appSwReserve5,
	mysqlpp::sql_varchar,	nswp_appSwReserve6,
	mysqlpp::sql_varchar,	nswp_appSwReserve7,
	mysqlpp::sql_varchar,	nswp_appSwReserve8,
	mysqlpp::sql_varchar,	nswp_appSwReserve9,
	mysqlpp::sql_varchar,	nswp_appSwReserve10,
	mysqlpp::sql_varchar,	nswp_appSwReserve11,
	mysqlpp::sql_varchar,	nswp_appSwReserve12,
	mysqlpp::sql_varchar,	nswp_appSwReserve13,
	mysqlpp::sql_varchar,	nswp_appSwReserve14,
	mysqlpp::sql_varchar,	nswp_appSwReserve15,
	mysqlpp::sql_varchar,	nswp_appSwReserve16,
	mysqlpp::sql_varchar,	nswp_appSwReserve17,*/
	mysqlpp::Null<mysqlpp::sql_varchar>,	nswp_description
);

