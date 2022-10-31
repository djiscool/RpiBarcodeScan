#ifndef PIBARCODEPROJ_H
#define PIBARCODEPROJ_H

#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define DEBUG 1

// START mysql config
// Placement for acquiring from config file
// Configure connection for mysql database here
// in the future we can parse a config file for this information

#define HOST "127.0.0.1" // you must keep the quotes on all four items,
#define USER "storage" // the function "mysql_real_connect" is looking for a char datatype,
#define PASSWD "d75kghosT" // without the quotes they're just an int.
#define DB "test"

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
//MYSQL_TIME ts; DEPRECATED
int query_state;

// END globals


// resets queries to their original state
// placement for reacquiring from config file
void reset_vars() {
    sub_query = "select sub_one('?')";
    add_query = "select add_one('?')";
    selAll = "SELECT * FROM " + TABLE;
}


int upc_count(std::string barcode) {

    if (DEBUG)
        std::cout << "DEBUG: In upc_name" << std::endl;

    std::string newUpcCount = upcCount;
    MYSQL_STMT *sql_statement_update;
    MYSQL_BIND sql_bind_parameters1[1];
    bool sql_error = false;


    // append barcode to query string
    std::size_t pos = newUpcCount.find("?");
    if (pos == std::string::npos) {
        std::cout << "Could not find placement for variable in sql statement"
                  << std::endl << "Please check sql statement for correctness"
                  << std::endl;
    } else {
        newUpcCount.replace(pos,1,barcode);

    }
    if (DEBUG)
        std::cout << newUpcCount << std::endl;

//Send query to database
    query_state = mysql_query(connection, newUpcCount.c_str());
// store result
    result = mysql_store_result(connection);
    if ((row = mysql_fetch_row(result)) == NULL ) {
        std::cout << "ERROR: Expected a returned row here." << std::endl;

    }
    int fail = atoi(row[0]);
    if (fail == 0) {

        std::cout << "An error occured, the product may be missing from inventory." << std::endl;
        std::cout << "This barcode has been logged. Contact Administrator for more details." << std::endl;
        return -1;
    }
    return fail;
}

std::string upc_name(std::string barcode) {

    if (DEBUG)
        std::cout << "DEBUG: In upc_name" << std::endl;

    std::string newUpcName = upcName;
    MYSQL_STMT *sql_statement_update;
    MYSQL_BIND sql_bind_parameters1[1];
    bool sql_error = false;


    // append barcode to query string
    std::size_t pos = newUpcName.find("?");
    if (pos == std::string::npos) {
        std::cout << "Could not find placement for variable in sql statement"
                  << std::endl << "Please check sql statement for correctness"
                  << std::endl;
    } else {
        newUpcName.replace(pos,1,barcode);

    }
    if (DEBUG)
        std::cout << newUpcName << std::endl;

//Send query to database
    query_state = mysql_query(connection, newUpcName.c_str());
// store result
    result = mysql_store_result(connection);
    if ((row = mysql_fetch_row(result)) == NULL ) {
        std::cout << "ERROR: Expected a returned row here." << std::endl;

    }
    std::string fail = row[0];
    if (fail == "Unknown") {

        std::cout << "An error occured, the product may be missing from inventory." << std::endl;
        std::cout << "This barcode has been logged. Contact Administrator for more details." << std::endl;
    }
    return row[0];
}


// This function removes 1 "can" of product from set table,
// given the UPC of said "can" supplemented by mysql function
// sub_one(varchar barcode)
void sub_one(std::string barcode) {
    reset_vars();
    if (DEBUG)
        std::cout << "DEBUG: In sub_one" << std::endl;

    MYSQL_STMT *sql_statement_update;
    MYSQL_BIND sql_bind_parameters1[1];
    bool sql_error = false;


    // append barcode to query string
    std::size_t pos = sub_query.find("?");
    if (pos == std::string::npos) {
        std::cout << "Could not find placement for variable in sql statement"
                  << std::endl << "Please check sql statement for correctness"
                  << std::endl;
    } else {
        sub_query.replace(pos,1,barcode);

    }
    if (DEBUG)
        std::cout << sub_query << std::endl;

//Send query to database
    query_state = mysql_query(connection, sub_query.c_str());
// store result
    result = mysql_store_result(connection);
    if ((row = mysql_fetch_row(result)) == NULL ) {
        std::cout << "ERROR: Expected a returned row here." << std::endl;

    }
    std::string fail = row[0];
    if (fail == "Failure") {

        std::cout << "An error occured, the product may be missing from inventory." << std::endl;
        std::cout << "The barcode has been logged. You can still take the product." << std::endl;
        return;
    }
    std::cout << "Sucessfully removed 1 of Product: " << row[0] << std::endl;
}



// prints all entries in set table
void read_database() {
    reset_vars();
    std::cout << "DEBUG: In read_database" << std::endl;

//Send query to database
    query_state = mysql_query(connection, selAll.c_str());
// store result
    result = mysql_store_result(connection);
    while ( ( row = mysql_fetch_row(result)) != NULL ) {
// Print result, it prints row[column_number])
        cout << row[0] << "\t" << row[1] << endl;
    }
}


// Adds one prodcut of given barcode, supplemented by a mysql function
// called add_one(varchar barcode)
void add_one(std::string barcode) {
    reset_vars();
    if (DEBUG)
        std::cout << "DEBUG: In add_one" << std::endl;

    MYSQL_STMT *sql_statement_add;
    MYSQL_BIND sql_bind_parameters1[1];
    bool sql_error = false;

    // append barcode to query string
    std::size_t pos = add_query.find("?");
    if (pos == std::string::npos) {
        std::cout << "Could not find placement for variable in sql statement"
                  << std::endl << "Please check sql statement for correctness"
                  << std::endl;
    } else {
        add_query.replace(pos,1,barcode);

    }

    if (DEBUG)
        std::cout << add_query << std::endl;

//Send query to database
    query_state = mysql_query(connection, add_query.c_str());
// store result
    result = mysql_store_result(connection);
    if ((row = mysql_fetch_row(result)) == NULL ) {
        std::cout << "ERROR: Expected a returned row here." << std::endl;

    }
    std::string fail = row[0];
    if (fail == "Failure") {

        std::cout << "An error occured, the product may be missing from inventory." << std::endl;
        std::cout << "The barcode has been logged. You can still take the product." << std::endl;
        return;
    }
    std::cout << "Sucessfully added 1 of Product: " << row[0] << std::endl;
}



#endif

