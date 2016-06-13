#ifndef PISCANSQLFUNCTIONS_H
#define PISCANSQLFUNCTIONS_H

#include <mysql/mysql.h>
#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>

#define DEBUG 0

// START mysql config

class MySQLFunctions {

private:

// DB connection variables
	const std::string HOST;
	const std::string USER;
	const std::string PASSWD;
	const std::string DB;

// define table here
    std::string TABLE = "inventory";

// define mysql functions here
// leave "?" where variables should go
    std::string sub_query = "select sub_one('?')";
    std::string add_query = "select add_one('?')";
    std::string selAll = "SELECT * FROM " + TABLE;
    std::string upcName = "SELECT upc_name('?')";
    std::string upcCount = "SELECT upc_count('?')";
// END mysql config

// MYSQL Globals
    MYSQL *connection, mysql;
    MYSQL_RES *result;
    MYSQL_ROW row;
    int query_state;
// END globals

public:

    MySQLFunctions(std::string table,std::string host,
	std::string user, std::string passwd, std::string db) : HOST(host),
	USER(user), PASSWD(passwd), DB(db), TABLE(table) {};
	~MySQLFunctions();
	int connect();
	int disconnect();
    void reset_vars();
	std::string oneSQLCall(std::string functionName, std::string barcode);
    int upc_count(std::string barcode);
    std::string upc_name(std::string barcode);
    void sub_one(std::string barcode);
    void read_database();
    void add_one(std::string barcode);

};


#endif
