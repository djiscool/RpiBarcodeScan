#ifndef GETBARCODE_H
#define GETBARCODE_H
/* This gets input from a barcode scanner, it is assumed that there is a
 function that does this, and returns the scanned value.
*/

#include <iostream>
#include <string>
#define DEBUG_MAIN 0

using namespace std;

std::string getScan() {


void parseConfig(std::string filename, std::map<std::string,std::string> &varMap) {

    std::ifstream ifs;

    if (DEBUG_MAIN) {
        std::cout << "filename: " << filename << endl;
    }

    ifs.open(filename.c_str(), std::ifstream::in);

    if (DEBUG_MAIN) {
        std::cout << "file opened" << endl;
    }
    std::string tempKey;
    std::string tempVar;



    std::string throwAway;
    int varIdx = 0;
    bool getKey = true;

    if (DEBUG_MAIN) {
        std::cout << "vars set" << endl;
    }
    char c = ifs.get();


//std::cout << "got first char" << endl;

// get variables from file..

    while (ifs.good()) {

        if (c == ' ') {
            if (DEBUG_MAIN) {
                std::cout << "empty line, continuing" << endl;
            }
            c = ifs.get();
            continue;
        }
        if ( c == '#') { // # indicates start of a comment line in config,
//				 ignore.
            std::getline(ifs,throwAway);
            if (DEBUG_MAIN) {
                std::cout << "got throw away line" << endl;
            }
        } else {

            if ( c == '\n') {
                // if end of line move onto next variable
                if (DEBUG_MAIN) {
                    std::cout << "final tempkey: " << tempKey << endl;
                    std::cout << "final tempVar: " << tempVar << endl;
                }
                getKey = true;
                varMap.insert( std::pair<std::string, std::string>(tempKey, tempVar));

                if (DEBUG_MAIN) {
                    //test print if values were inserted into map
                    auto it = varMap.find(tempKey.c_str());
                    if (it != varMap.end()) {
                        std::cout << it->first<<" =>"<< it->second << '\n';
                    }
                }
                //clear temp values for next variable
                tempVar = "";
                tempKey = "";
                varIdx++;
            } else if (getKey) {
                if (c == '=') {
                    getKey = false;
                } else {
                    tempKey += c;
                }
            } else {
                tempVar += c;
            }
        }
        c = ifs.get();
        if (DEBUG_MAIN) {
            std::cout << "tempKey: " << tempKey << endl;
        }
    }

    ifs.close();

    if (DEBUG_MAIN) {
        std::cout << "file closed" << endl;
    }

    if (DEBUG_MAIN) {
// showing contents:
        std::map<std::string,std::string>::iterator it;
        std::cout << "mymap contains:\n";
        for (it=varMap.begin(); it!=varMap.end(); ++it) {
            std::cout << it->first << " => " << it->second << '\n';
        }
    }
}

#endif
