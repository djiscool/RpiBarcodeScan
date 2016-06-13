#include "piScanSQLFunctions.h"

MySQLFunctions::~MySQLFunctions() {
	disconnect();
}

// initializes database connection
int MySQLFunctions::connect() {
	
	
		//	initialize database connection (we want to do this after recieving input
	// 	to make sure we don't time out)
	//	mysql is global to the class, so we don't need to pass it

        mysql_init(&mysql);

// 	The three zeros are: Which port to connect to, which socket to connect
// 	to and what client flags to use.  unless you're changing the defaults
// 	you only need to put 0 here
        connection = mysql_real_connect(&mysql,HOST.c_str(),USER.c_str(),PASSWD.c_str(),DB.c_str(),0,0,0);
// 	Report error if failed to connect to database
        if (connection == NULL) {
            std::cout << mysql_error(&mysql) << std::endl;
            return 1;
        }
	return 0;
}

// closes connection to database
int MySQLFunctions::disconnect() {
		// Close our database connection
        mysql_close(&mysql);
	
}

// resets queries to their original state
// placement for reacquiring from config file
void MySQLFunctions::reset_vars() {
    sub_query = "select sub_one('?')";
    add_query = "select add_one('?')";
    selAll = "SELECT * FROM " + TABLE;
}



std::string MySQLFunctions::oneSQLCall(std::string functionName, std::string barcode) {

    if (DEBUG)
        std::cout << "DEBUG: Function name: functionName" << std::endl;
    // create sql statement
    std::string functionStatement = "SELECT " + functionName + "('" + barcode
	+ "')";
    MYSQL_STMT *sql_statement_update;
    MYSQL_BIND sql_bind_parameters1[1];
    bool sql_error = false;
	if(DEBUG) {
	std::cout << "Vars set" << std::endl;
	std::cout << "functionStatement =: " << functionStatement << std::endl;
}

//Send query to database
    query_state = mysql_query(connection,functionStatement.c_str());
if(DEBUG) {
	std::cout << "Query sent" << std::endl;
}
// store result
    result = mysql_store_result(connection);
	if(DEBUG) {
		std::cout << "Result stored" << std::endl;
	}
    if ((row = mysql_fetch_row(result)) == NULL ) {
        std::cout << "ERROR: Expected a returned row here." << std::endl;

    }
	if(DEBUG) {
		std::cout << "row fetched" << std::endl;
}
    std::string fail = row[0];
    if (fail == "Unknown") {

        std::cout << "An error occured, the product may be missing from inventory." << std::endl;
        std::cout << "This barcode has been logged. Contact Administrator for more details." << std::endl;
    }
	if(DEBUG) {
	std::cout << "Got result" << std::endl;
}
    return row[0];
}


int MySQLFunctions::upc_count(std::string barcode) {

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

std::string MySQLFunctions::upc_name(std::string barcode) {

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
void MySQLFunctions::sub_one(std::string barcode) {
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
if(DEBUG) {
	std::cout << "got query_state" << std::endl;
}
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
void MySQLFunctions::read_database() {
    reset_vars();
    std::cout << "DEBUG: In read_database" << std::endl;

//Send query to database
    query_state = mysql_query(connection, selAll.c_str());
// store result
    result = mysql_store_result(connection);
    while ( ( row = mysql_fetch_row(result)) != NULL ) {
// Print result, it prints row[column_number])
        std::cout << row[0] << "\t" << row[1] << std::endl;
    }
}


// Adds one prodcut of given barcode, supplemented by a mysql function
// called add_one(varchar barcode)
void MySQLFunctions::add_one(std::string barcode) {
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

