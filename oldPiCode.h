
/*	DEPRECATED

	This function assumes that a MYSQL_TIME variable named ts exists globally

	setCurrentDateTime sets the ts variable to the current time and date of when

		function is called.


// Get current date/time, format is YYYY-MM-DD HH:mm:ss
void setCurrentDateTime() {
    std::cout << "In setCurrentDateTime" << std::endl;

    time_t     now = time(0);
    struct tm  tstruct;

    std::cout << "hour: " << tstruct.tm_hour << std::endl;
    std::cout << "min: " << tstruct.tm_min << std::endl;
    std::cout << "sec: " << tstruct.tm_sec << std::endl;

    // ts is the time struct for the query, we assume its global here.
    ts.year= tstruct.tm_year + 1900; // tm_year is years since 1900, so we add 1900 to it to get the correct year
    ts.month= tstruct.tm_mon + 1; // month starts at 0, so add 1 to get correct month
    ts.day= tstruct.tm_mday;
    ts.hour= tstruct.tm_hour; // hour starts at 0
    ts.minute= tstruct.tm_min; //min starts at 0
    ts.second= tstruct.tm_sec;


}

*/



/* DEPRECATED
// Checks if a UPC is in the database, returns TRUE if exists.
bool exists_upc(std::string upc) {
    std::cout << "In exists_upc" << std::endl;

    std::string query = "select * from " + TABLE + " where upc='" + upc +"'";
    bool existance = true;
    std::cout << "query = " << query.c_str() << std::endl;
//Send query to database
    query_state = mysql_query(connection, query.c_str());
    std::cout <<"query_state(nonzero if error occured): " << query_state << std::endl;
// store result
    result = mysql_store_result(connection);
    if (*mysql_error(&mysql)) {
        cerr << "An error occured in select query " << mysql_errno(&mysql);
    }

    //while ( ( row = mysql_fetch_row(result)) != NULL )
    std::cout << "num rows: " << mysql_num_rows(result) << std::endl;
    if (mysql_num_rows(result) < 1 ) {
        existance = false;
    }

    return existance;
}

*/



/*

// DEPRECATED, Update_database used this, but is no longer needed with the new SQL function
// deletes an entry from table given its barcode, and exp_date
void delete_entry(std::string barcode, std::string exp_date) {
    std::cout << "In delete_entry" << std::endl;

    MYSQL_STMT *sql_statement3;
    MYSQL_BIND sql_bind_parameters3[2];
    bool sql_error = false;


    //Setup to create query
    sql_statement3= mysql_stmt_init(&mysql);
    if (!sql_statement3) {
        sql_error = 1;
        std::cout << "Initialize mysql statement failed" << std::endl;
    }


    //----- SET THE QUERY TEXT -----
    // may or may not work
std::string SQL_QUERY_TEXT_3 "DELETE FROM " + TABLE + " WHERE UPC=? AND exp_date=?";
    if (mysql_stmt_prepare(sql_statement3, SQL_QUERY_TEXT_3, strlen(SQL_QUERY_TEXT_3))) {
        sql_error = 1;
        std::cout << "mysql prepare statement failed" << std::endl;
        std::cout << mysql_stmt_error(sql_statement3) << std::endl;
    }

    //----- SET THE QUERY PARAMETER VALUES -----
    //If you want to know how many parameters are expected
    //int param_count = mysql_stmt_param_count(sql_statement3);

    //Set the parameter values
    memset(sql_bind_parameters3, 0, sizeof(sql_bind_parameters3));		//Reset the parameters memory to null

    //UPC (String)
    char upc[50];
    strcpy(upc, barcode.c_str());
    unsigned long upc_length = strlen(upc);
    sql_bind_parameters3[0].buffer_type = MYSQL_TYPE_STRING;
    sql_bind_parameters3[0].buffer = (char*)upc;
    sql_bind_parameters3[0].buffer_length = sizeof(upc);
    sql_bind_parameters3[0].is_null = 0;
    sql_bind_parameters3[0].length = &upc_length;			//<Note: this is a pointer!


    //exp_date (String)
    unsigned long exp_length = strlen(exp_date.c_str());
    sql_bind_parameters3[1].buffer_type = MYSQL_TYPE_STRING;
    sql_bind_parameters3[1].buffer = (char*)exp_date.c_str();
    sql_bind_parameters3[1].buffer_length = sizeof(exp_date.c_str());
    sql_bind_parameters3[1].is_null = 0;
    sql_bind_parameters3[1].length = &exp_length;			//<Note: this is a pointer!

    //Pointers are used in the bind parameters so that if you are say adding multiple rows you can use the same query setup with new values for each execute of it.

    //Bind the buffers
    if (mysql_stmt_bind_param(sql_statement3, sql_bind_parameters3)) {
        sql_error = 1;
        std::cout << "Bind Failed" << std::endl;
    }

    //----- EXECUTE THE QUERY ------
    if (!sql_error) {
        if (DEBUG)
            std::cout << "Executing Statement" << std::endl;
        if (mysql_stmt_execute(sql_statement3))
            sql_error = 1;
    }

    //If you want to get the number of affected rows
    my_ulonglong affected_rows = mysql_stmt_affected_rows(sql_statement3);
    if (affected_rows != 1) {
        std::cout << "Affected Rows: " << affected_rows << std::endl;
    }

    //IF YOU WANT TO GET THE VALUE GENERATED FOR AN AUTO_INCREMENT COLUMN IN THE PREVIOUS INSERT/UPDATE STATEMENT
    //my_ulonglong sql_insert_id = mysql_stmt_insert_id(sql_statement3);

    //If you want to do the query again then change any values you want to change and call mysql_stmt_execute(sql_statement3) again

    //Close the statement
    if (sql_statement3) {
        if (mysql_stmt_close(sql_statement3))
            sql_error = 1;
    }

}

*/


    /*


	OLD UPDATE FUNCTION

    This is old code(it works, but there is a sql function that does the same)

    // Assuming Removing 1 Can
        int cans_remain;

        std::string query = "select cans_left,exp_date from " + TABLE + " where upc='" + barcode + "'";
    //Send query to database
        query_state = mysql_query(connection, query.c_str());
    // store result
        result = mysql_store_result(connection);
        if ((row = mysql_fetch_row(result)) == NULL )
        {
            std::cout << "expected a returned row here" << std::endl;
        }
        cans_remain = atoi(row[0]);

        std::cout << "cans_left from " << barcode << " : " << cans_remain << std::endl;
        //if first row is empty update second row, and then delete the first
        bool is_zero = false;
        bool deleteBoth = false;
        std::string deleteDate;
        if (cans_remain < 1)
        {
            is_zero = true;
            deleteDate = row[1];
            std::cout << "no more cans, exp_date = " << row[1] << std::endl;

            //get next row for update

            if ((row = mysql_fetch_row(result)) == NULL )
            {
                std::cout << "There are no more of this product" << std::endl;
                delete_entry(barcode,deleteDate);
                return;
            }

            cans_remain = atoi(row[0]);

            std::cout << "cans_left from " << barcode << " : " << cans_remain << std::endl;
            if ((cans_remain - 1) < 1)   //if we empty the entry we want to delete it.
            {
                deleteBoth = true;
            }
        }
        else if((cans_remain -1 ) < 1)
        {
            is_zero = true;
    		deleteDate = row[1];
        }


        // Update database
        if (!deleteBoth)
        {

            MYSQL_STMT *sql_statement_update;
            MYSQL_BIND sql_bind_parameters1[3];
            bool sql_error = false;

            //Setup to create query
            sql_statement_update= mysql_stmt_init(&mysql);
            if (!sql_statement_update)
            {
                sql_error = 1;
                std::cout << "Initialize mysql statement failed" << std::endl;
            }


            //----- SET THE QUERY TEXT -----
    	std::string SQL_QUERY_TEXT_2 = "UPDATE " + TABLE + " SET cans_left=? WHERE UPC=? AND exp_date=?";
            if (mysql_stmt_prepare(sql_statement_update, SQL_QUERY_TEXT_2, strlen(SQL_QUERY_TEXT_2)))
            {
                sql_error = 1;
                std::cout << "mysql prepare statement failed" << std::endl;
            }

            //----- SET THE QUERY PARAMETER VALUES -----
            //If you want to know how many parameters are expected
            //int param_count = mysql_stmt_param_count(sql_statement1);

            //Set the parameter values
            memset(sql_bind_parameters1, 0, sizeof(sql_bind_parameters1));		//Reset the parameters memory to null

            cans_remain--;
            std::cout <<"Expected cans after removal: " << cans_remain << std::endl;
            // cans_left (Integer)
            sql_bind_parameters1[0].buffer_type = MYSQL_TYPE_LONG;
            sql_bind_parameters1[0].buffer = (char*)&cans_remain;		//<Note: this is a pointer!
            sql_bind_parameters1[0].is_null = 0;
            sql_bind_parameters1[0].length = 0;

            //UPC (String)
            char upc[50];
            strcpy(upc, barcode.c_str());
            unsigned long upc_length = strlen(upc);
            sql_bind_parameters1[1].buffer_type = MYSQL_TYPE_STRING;
            sql_bind_parameters1[1].buffer = (char*)upc;
            sql_bind_parameters1[1].buffer_length = sizeof(upc);
            sql_bind_parameters1[1].is_null = 0;
            sql_bind_parameters1[1].length = &upc_length;			//<Note: this is a pointer!


            //exp_date (String), row[1] contains the exp_date we got from the database
            unsigned long exp_length = strlen(row[1]);
            sql_bind_parameters1[2].buffer_type = MYSQL_TYPE_STRING;
            sql_bind_parameters1[2].buffer = (char*)row[1];
            sql_bind_parameters1[2].buffer_length = sizeof(row[1]);
            sql_bind_parameters1[2].is_null = 0;
            sql_bind_parameters1[2].length = &exp_length;			//<Note: this is a pointer!

            //Pointers are used in the bind parameters so that if you are say adding multiple rows you can use the same query setup with new values for each execute of it.

            //Bind the buffers
            if (mysql_stmt_bind_param(sql_statement_update, sql_bind_parameters1))
            {
                sql_error = 1;
                std::cout << "Bind Failed" << std::endl;
            }

            //----- EXECUTE THE QUERY ------
            if (!sql_error)
            {
                if (DEBUG)
                    std::cout << "Executing Statement" << std::endl;
                if (mysql_stmt_execute(sql_statement_update))
                {
                    sql_error = 1;
                    std::cout << "Execute Statement Failed" << std::endl;
                }

            }

            //If you want to get the number of affected rows
            my_ulonglong affected_rows = mysql_stmt_affected_rows(sql_statement_update);
            if (affected_rows != 1)
            {
                std::cout << "Affected Rows: " << affected_rows << std::endl;
            }

            //IF YOU WANT TO GET THE VALUE GENERATED FOR AN AUTO_INCREMENT COLUMN IN THE PREVIOUS INSERT/UPDATE STATEMENT
            //my_ulonglong sql_insert_id = mysql_stmt_insert_id(sql_statement1);

            //If you want to do the query again then change any values you want to change and call mysql_stmt_execute(sql_statement1) again

            //Close the statement
            if (sql_statement_update)
            {
                if (mysql_stmt_close(sql_statement_update))
                    sql_error = 1;
            }
            if(is_zero)
            {
                // delete entry where exp_date = deleteDate
                delete_entry(barcode, deleteDate);
            }
        }
        else
        {
            // delete both entries
            delete_entry(barcode,deleteDate);
            delete_entry(barcode,row[1]);
        }
    	*/

}


/* 	This is a function for refence.

	read_database prints everything in TABLE
*/



    /*
    else {
        std::cout <<"barcode " << barcode << " doesn't exist" << std::endl;

        // Get exp_date
        std::string exp_date;
        std::cout << "enter expiration date (YYYY-MM-DD): ";
        std::cin >> exp_date;
        // Get cans_left
        int cans_left;
        std::cout << "enter how many cans: ";
        std::cin >> cans_left;

        MYSQL_STMT *sql_statement1;
        MYSQL_BIND sql_bind_parameters1[4];
        bool sql_error = false;


        //Setup to create query
        sql_statement1= mysql_stmt_init(&mysql);
        if (!sql_statement1) {
            sql_error = 1;
            std::cout << "Initialize mysql statement failed" << std::endl;
        }


        //----- SET THE QUERY TEXT -----

    	// may or may not work

    	std::string SQL_QUERY_TEXT_1 = "INSERT INTO " + TABLE + "(UPC, exp_date,cans_left,date_created) VALUES(?,?,?,?)";
        if (mysql_stmt_prepare(sql_statement1, SQL_QUERY_TEXT_1, strlen(SQL_QUERY_TEXT_1))) {
            sql_error = 1;
            std::cout << "mysql prepare statement failed" << std::endl;
        }

        //----- SET THE QUERY PARAMETER VALUES -----
        //If you want to know how many parameters are expected
        //int param_count = mysql_stmt_param_count(sql_statement1);

        //Set the parameter values
        memset(sql_bind_parameters1, 0, sizeof(sql_bind_parameters1));		//Reset the parameters memory to null

        //UPC (String)
        char upc[50];
        strcpy(upc, barcode.c_str());
        unsigned long upc_length = strlen(upc);
        sql_bind_parameters1[0].buffer_type = MYSQL_TYPE_STRING;
        sql_bind_parameters1[0].buffer = (char*)upc;
        sql_bind_parameters1[0].buffer_length = sizeof(upc);
        sql_bind_parameters1[0].is_null = 0;
        sql_bind_parameters1[0].length = &upc_length;			//<Note: this is a pointer!


        //exp_date (String)
        unsigned long exp_length = strlen(exp_date.c_str());
        sql_bind_parameters1[1].buffer_type = MYSQL_TYPE_STRING;
        sql_bind_parameters1[1].buffer = (char*)exp_date.c_str();
        sql_bind_parameters1[1].buffer_length = sizeof(exp_date.c_str());
        sql_bind_parameters1[1].is_null = 0;
        sql_bind_parameters1[1].length = &exp_length;			//<Note: this is a pointer!

        // cans_left (Integer)
        sql_bind_parameters1[2].buffer_type = MYSQL_TYPE_LONG;
        sql_bind_parameters1[2].buffer = (char*)&cans_left;		//<Note: this is a pointer!
        sql_bind_parameters1[2].is_null = 0;
        sql_bind_parameters1[2].length = 0;


        //date time
        sql_bind_parameters1[3].buffer_type= MYSQL_TYPE_DATETIME;
        sql_bind_parameters1[3].buffer= (char *)&ts;
        sql_bind_parameters1[3].is_null= 0;
        sql_bind_parameters1[3].length= 0;
        // the tutorial binds before we set these values, so we shall do the same

        //Pointers are used in the bind parameters so that if you are say adding multiple rows you can use the same query setup with new values for each execute of it.

        //Bind the buffers
        if (mysql_stmt_bind_param(sql_statement1, sql_bind_parameters1)) {
            sql_error = 1;
            std::cout << "Bind Failed" << std::endl;
        }

        //set time values
        setCurrentDateTime();

        //----- EXECUTE THE QUERY ------
        if (!sql_error)
        {
            if (DEBUG)
                std::cout << "Executing Statement" << std::endl;
            if (mysql_stmt_execute(sql_statement1))
                sql_error = 1;
        }

        //If you want to get the number of affected rows
        my_ulonglong affected_rows = mysql_stmt_affected_rows(sql_statement1);
        if (affected_rows != 1)
        {
            std::cout << "Affected Rows: " << affected_rows << std::endl;
        }

        //IF YOU WANT TO GET THE VALUE GENERATED FOR AN AUTO_INCREMENT COLUMN IN THE PREVIOUS INSERT/UPDATE STATEMENT
        //my_ulonglong sql_insert_id = mysql_stmt_insert_id(sql_statement1);

        //If you want to do the query again then change any values you want to change and call mysql_stmt_execute(sql_statement1) again

        //Close the statement
        if (sql_statement1)
        {
            if (mysql_stmt_close(sql_statement1))
                sql_error = 1;
        }

    }
    */
