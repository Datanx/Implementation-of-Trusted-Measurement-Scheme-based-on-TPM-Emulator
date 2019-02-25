/* 
 * File:   main.cpp
 * Author: yichuan
 *
 * Created on 2009年11月25日, 下午4:13
 */

#include <stdlib.h>

#include "MySqlDriver.h"
/*
 * 
 */
int main(int argc, char** argv) {

    MySqlDriver tmysql = MySqlDriver("127.0.0.1","root","mysql","TRUST_COMPUTING_DATABASE",3306);
    tmysql.MySqlDriverConnect();
    tmysql.MySqlDriverInsert("BIOS","(ID,BIOS_SN)","(1,'xyz')");
    tmysql.MySqlDriverDelete("BIOS","ID=2");
    tmysql.MySqlDriverUpdate("BIOS","BIOS_SN","'bbb'","ID=1");
    tmysql.MySqlDriverQuery("ID,BIOS_SN","BIOS");
    tmysql.MySqlDriverFetchRow();
    std::list<MYSQL_ROW>::iterator it = tmysql.rowList.begin();
    while(it != tmysql.rowList.end())
    {
        char** tmp = *it;
        cout << *tmp << "|" << *(tmp+1) << endl;
        it++;
    }
    

    tmysql.MySqlDriverDisconnect();





    return (EXIT_SUCCESS);
}

