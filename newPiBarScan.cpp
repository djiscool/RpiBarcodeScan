#include "newPiBarScan.h"


int handleSQLold(std::string HOST, std::string USER, std::string PASSWD, std::string DB,
                 std::string TABLE) {
    const int ADDONE = 1;
    const int READ = 2;
    const int SUBONE = 3;
    const int UPCCOUNT = 4;

    // create database instance
    MySQLFunctions myFuncs(TABLE, HOST, USER, PASSWD, DB);

    int query_type = SUBONE;

    while (true) { // Loop forever (wait for input from barcode scanner)

        std::string barcode;
        // 	Get Barcode
        barcode = getScan();


        // initialze db connection
        // we want to do this after recieving input
        // to make sure we don't time out
        if (myFuncs.connect() == 0) {

            if (DEBUG_MAIN) {
                std::cout << "Barcode: " << barcode << std::endl;

                query_type = ADDONE;
            }

            if (barcode == "ADDONE") {
                query_type = ADDONE;
                continue;
            } else if (barcode == "SUBONE") {
                query_type = SUBONE;
                continue;
            } else if (barcode == "UPCCOUNT") {
                query_type = UPCCOUNT;
                continue;
            }

            if (DEBUG_MAIN)
                std::cout << "query_type = " << query_type << std::endl;


// switch here
            switch (query_type) {
            case ADDONE:
                myFuncs.add_one(barcode);
                break;
            case READ:
                myFuncs.read_database();
                break;
            case SUBONE:
                myFuncs.sub_one(barcode);
                break;
            case UPCCOUNT:
                int num_cans = -1;
                num_cans = myFuncs.upc_count(barcode);
                if (num_cans == -1) {
                    std::cout << "An error occured, exiting UPCCOUNT and resuming normal operation" << std::endl;
                    continue;
                }
                std::string barcode_name;
                barcode_name = myFuncs.upc_name(barcode);
                if (barcode_name == "Unknown" || barcode_name.empty()) {
                    std::cout << "An error occured, the name may not be in the database" << std::endl;
                    std::cout << "exiting UPCCOUNT, and resuming normal operation" << std::endl;
                }
                std::cout << "There are " << num_cans << " of " << barcode_name << std::endl;

                continue;
            }
        } // end connection check
		else {
			std::cout << "connection failed, check configuration" << std::endl;
            return 1;
        } 

        myFuncs.disconnect();
        query_type = SUBONE; // set query type back to default
    } // END LOOP

    return 0;
}

int handleSQLnew(std::string HOST, std::string USER, std::string PASSWD,
 	std::string DB, std::string TABLE, std::string sqlFunctionName) {

    // create database instance
    MySQLFunctions myFuncs(TABLE, HOST, USER, PASSWD, DB);


    while (true) { // Loop forever (wait for input from barcode scanner)

        std::string barcode;
        // 	Get Barcode
        barcode = getScan();


        // initialze db connection
        // we want to do this after recieving input
        // to make sure we don't time out
        if (myFuncs.connect() == 0) {

            if (DEBUG_MAIN) {
                std::cout << "Barcode: " << barcode << std::endl;
            }
		std::cout << myFuncs.oneSQLCall(sqlFunctionName,
			 barcode) << std::endl; 		


	
        } else {
            std::cout << "connection failed, check configuration" << std::endl;
            return 1;
        }
        myFuncs.disconnect();

    }
    return 0;
}


int main() {


// 	parse config file for config
    std::string config_filename = "piBarScan.config";

    std::map<std::string,std::string> varMap;
    parseConfig(config_filename, varMap);
    std::map<std::string,std::string>::iterator it;
    it = varMap.find("MysqlFunctionName");

    if (DEBUG_MAIN) {
        std::cout << "mysqlfunctionname (by array access): " << varMap["MysqlFunctionName"] << endl;
    }

    int returnVal = 0;

    if ( it == varMap.end() || it->second == "\n" || it->second == "" ) {
        // not found
        std::cout << "Mysql function name not found, using old mysql method. \n If you think this is an error check configuration file." << endl;
        returnVal = handleSQLold(varMap["Hostname"], varMap["Username"], varMap["Password"],
                                 varMap["DB"], varMap["Table"]);
    } else {
        // found
        std::cout << "config found, using " << it->second << endl;
        returnVal = handleSQLnew(varMap["Hostname"], varMap["Username"], varMap["Password"],
                                 varMap["DB"], varMap["Table"], varMap["MysqlFunctionName"]);
    }
//std::string HOST, std::string USER, std::string PASSWD, std::string DB
    return returnVal;


}
