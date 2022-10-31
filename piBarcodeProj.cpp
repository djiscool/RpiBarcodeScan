/* This program will recieve input from barcode scanner and submit information to database depending on the input, this manages food storage inventory.
For example scan a barcode that says we want to add inventory, you will then scan the product, and possibly scan a qty barcode, then this will be thrown into a mysql database.

Some queries used to set up the databse, shown below, can be used as examples.

USE STORAGE;
DROP TABLE inventory;
DROP TABLE product;
DROP TABLE storage_log;
SHOW TABLES;
SHOW COLUMNS FROM inventory;
SHOW COLUMNS FROM product;

CREATE TABLE `inventory` (
  `UPC` varchar(100) NOT NULL,
  `exp_date` date NOT NULL,
  `cans_left` int(11) NOT NULL DEFAULT '0',
  `date_created` datetime DEFAULT NULL,
  PRIMARY KEY (`UPC`,`exp_date`),
  CONSTRAINT `UPC` FOREIGN KEY (`UPC`) REFERENCES `product` (`UPC`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `product` (
  `UPC` varchar(100) NOT NULL,
  `display_name` varchar(100) DEFAULT NULL,
  `category` varchar(100) DEFAULT NULL,
  `brand` varchar(100) DEFAULT NULL,
  `product_name` varchar(100) DEFAULT NULL,
  `product_variety` varchar(100) DEFAULT NULL,
  `oz` float DEFAULT NULL,
  PRIMARY KEY (`UPC`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


CREATE TABLE `storage_log` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `UPC` varchar(100) DEFAULT NULL,
  `exp_date` date DEFAULT NULL,
  `event_date` datetime NOT NULL,
  `event_type` varchar(100) DEFAULT NULL,
  `event_status` varchar(100) DEFAULT NULL,
  `starting_value` int(11) DEFAULT NULL,
  `ending_value` int(11) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=34 DEFAULT CHARSET=latin1;


INSERT INTO inventory(UPC, exp_date, cans_left, date_created)
	VALUES('1132564332','2016-12-12', 12, NOW());
INSERT INTO product(UPC, display_name, product_name, product_variety, brand, oz)
	VALUES('1132564332', 'Green Beans', 'Green Beans', 'Mean', 'MeanGreens', 10);
INSERT INTO storage_log(UPC, exp_date, event_date, event_type, adjust_value)
	VALUES('1132564332', '2016-12-12',NOW(),'sqladminADD',12);

UPDATE inventory SET cans_left=11 WHERE UPC='1132564332';

SELECT * FROM inventory;
SELECT * FROM product;
SELECT * FROM storage_log;

DELETE FROM inventory WHERE UPC='1132584332';

END Query setup examples
*/

#include "getBarcode.h" // this has our barcode scanning function/s
#include "piBarcodeProj.h" // this has our sql helper functions
#include <iostream>

#define ADDONE 1
#define READ 2
#define SUBONE 3
#define UPCCOUNT 4


/* following code referenced from
https://www.raspberrypi.org/forums/viewtopic.php?f=33&t=31394
*/

using namespace std;



#define DEBUG_MAIN 0

int main() {
    int query_type = SUBONE;

    while (true) { // Loop forever (wait for input from barcode scanner)

        std::string barcode;
// Get Barcode
        barcode = getScan();

        if (DEBUG_MAIN) {
            std::cout << "Barcode: " << barcode << std::endl;
        }
        if (DEBUG_MAIN) {
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


//	initialize database connection (we want to do this after recieving input
//		to make sure we don't time out)
//	mysql is global, so we don't need to pass it

        mysql_init(&mysql);

// the three zeros are: Which port to connect to, which socket to connect to
// and what client flags to use.  unless you're changing the defaults you only need to put 0 here
        connection = mysql_real_connect(&mysql,HOST,USER,PASSWD,DB,0,0,0);
// Report error if failed to connect to database
        if (connection == NULL) {
            cout << mysql_error(&mysql) << endl;
            return 1;
        }

// switch here
        switch (query_type) {
        case ADDONE:
            add_one(barcode);
            break;
        case READ:
            read_database();
            break;
        case SUBONE:
            sub_one(barcode);
            break;
        case UPCCOUNT:
            int num_cans = -1;
            num_cans = upc_count(barcode);
            if (num_cans == -1) {
                std::cout << "An error occured, exiting UPCCOUNT and resuming normal operation" << std::endl;
                continue;
            }
            std::string barcode_name;
            barcode_name = upc_name(barcode);
            if (barcode_name == "Unknown" || barcode_name.empty()) {
                std::cout << "An error occured, the name may not be in the database" << std::endl;
                std::cout << "exiting UPCCOUNT, and resuming normal operation" << std::endl;
            }
            std::cout << "There are " << num_cans << " of " << barcode_name << std::endl;

            continue;
        }

        // Close our database connection
        mysql_close(&mysql);
        query_type = SUBONE; // set query type back to default
    } // END LOOP

    return 0;
}


