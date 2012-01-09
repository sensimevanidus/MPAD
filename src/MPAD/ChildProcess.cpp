/**
 * File: ChildProcess.cpp
 * 
 * Source code file for the ChildProcess class.
 * 
 * @author Onur Yaman <onuryaman@gmail.com>
 * @version 0.1
 * @since 2011-10-28
 */

// include standard libraries.
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

// include the string library for the std::string class.
#include <string.h>

// include the signature of the class.
#include "ChildProcess.h"

// include the Logger class signature.
#include "Logger.h"
#include "AlgorithmDES.h"
#include <math.h>
#include <vector>
#include <sstream>
#include <iostream>

/**
 * Main entry of the application.
 * 
 * @return The exit status of the application.
 */
int main(int argc, char **argv)
{
    // instantiate the ChildProcess class.
    new ChildProcess(std::string(argv[1]), std::string(argv[2]), std::string(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));
    //new ChildProcess(std::string("child process"), std::string("This is the raw string!"), std::string("T??*LY???pqXWSyJ??B^P???DE??????i?g?c!N2??ke??gkb???!??p?'P"), atoi("1"), atoi("56"), atoi("0"));
    
    // terminate the application.
    exit(EXIT_SUCCESS);
}

/**
 * Constructor method of the class.
 * 
 * @param name The name of the child process that is given by the main
 * process.
 * @param sleepTime The sleep time (in seconds) for the child process.
 * @param repeatCount The number of times the child process will repeat
 * the sleep/wake-up operations.
 */
ChildProcess::ChildProcess(std::string name, std::string rawString, std::string encryptedString, int algorithmId, int keyLength, int partitionNumber, int partitionSize)
{
    // mark the name of the process as an instance property.
    this->name = name;
    Logger::writeToLogFile("Child %s is created!", name);
    
    uint64_t start = partitionNumber * ((uint64_t) pow(2, keyLength) / pow(2, partitionSize));
    uint64_t end = start + ((uint64_t) pow(2, keyLength) / pow(2, partitionSize)) - 1;
    std::string decryptedString;
    
    for (uint64_t i = start; i < end; i++) {
        // -------------------------------------------------------------------------
        // @todo
        AlgorithmDES algoDES;
        // create the key.
        std::vector<unsigned char> key(56, '0');
        if (0 < partitionNumber) {
                key.at(partitionSize - partitionNumber) = '1';
        }
        for (int j = 0; j < keyLength; j++) {
            int bitValue = ((i >> j) & 1);
            if (0 == bitValue) {
                key.at(keyLength - j - 1) = '0';
            } else {
                key.at(keyLength - j - 1) = '1';
            }
        }
        
        decryptedString = algoDES.decrypt(encryptedString, (const char *)key.data(), keyLength);
        
        if (partitionNumber == 0 && rawString == decryptedString) {
            Logger::writeToLogFile("The key is found! Key is: %s", (const char*) key.data());
            
            exit(2);
        }
    }
}
