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

/**
 * Main entry of the application.
 * 
 * @return The exit status of the application.
 */
int main(int argc, char **argv)
{
    // instantiate the ChildProcess class.
    new ChildProcess(std::string(argv[1]), atoi(argv[2]), atoi(argv[3]));
    
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
ChildProcess::ChildProcess(std::string name, int sleepTime, int repeatCount)
{
    // mark the name of the process as an instance property.
    this->name = name;
    
    // log the start event.
    Logger::writeToLogFile("%s started!", this->name);
    
    // for the given repeatCount, halp the process.
    for (int i = 0; i < repeatCount; i++) {
        // halt the process for the given duration.
        this->sleepWell(sleepTime);
    }
    
    // log the terminate event.
    Logger::writeToLogFile("%s will be terminated!", this->name);
}

/**
 * Halts the current process for the given duration (in seconds).
 * 
 * @param duration The sleep time (in seconds) for the child process.
 */
void ChildProcess::sleepWell(int duration)
{
    // local variable for the log message.
    char messageBuffer[50];
    
    // prepare the log message.
    sprintf(messageBuffer, "%s will sleep for %d seconds!",
        (this->name).c_str(), duration
    );
    
    // log the sleep event.
    Logger::writeToLogFile(std::string(messageBuffer));
    
    // halt the process.
    sleep(duration);
    
    // log the wake-up event.
    Logger::writeToLogFile("%s woke up!", this->name);
}
