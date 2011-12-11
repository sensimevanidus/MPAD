/**
 * File: Logger.cpp
 * 
 * Source code file for the Logger class.
 * 
 * @author Onur Yaman <onuryaman@gmail.com>
 * @version 0.1
 * @since 2011-10-28
 */

// include the stream related libraries.
#include <iostream>
#include <fstream>
#include <sstream>

// include the string library for the std::string class.
#include <string.h>

// include the errno library for the errno variable.
#include <errno.h>

// include the time libraries for date/time related functions.
#include <time.h>
#include <sys/time.h>

// include the signature of the class.
#include "Logger.h"

// by default, the class is not instantiated.
bool Logger::instanceFlag = false;
Logger* Logger::instance = NULL;

/**
 * If the Logger class was instantiated before; returns a pointer to
 * that instance. If not; instantiates the class, stores a pointer to
 * the instance and returns it.
 * 
 * @return Pointer to the singleton instance.
 */
Logger* Logger::getInstance()
{
    // if the class was not instantiated before;
    if (! Logger::instanceFlag) {
        // instantiate it.
        Logger::instance = new Logger();
        
        // mark it as instantiated.
        Logger::instanceFlag = true;
    }

    // return the singleton instance.
    return Logger::instance;
}

/**
 * Writes a log message to the shared log file in a safe manner.
 * 
 * @param logMessage Event message that is going to be logged.
 */
void Logger::writeToLogFile(std::string logMessage) { 
    // open the log file in append method.
    std::ofstream logFile("mpad.log", std::ios::app);
    
    // if the file is open;
    if (logFile.is_open()) {
        // calculate the current data and time values.
        time_t rawTime;
        struct tm* timeInfo;
        char buffer[80];
        time(&rawTime);
        timeInfo = localtime(&rawTime);
        
        // format the date/time values.
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S.", timeInfo);
        timeval rawTimeInMicroSeconds;
        gettimeofday(&rawTimeInMicroSeconds, NULL);
        sprintf(buffer, "%s%d", buffer, (rawTimeInMicroSeconds.tv_usec / 1000) % 1000);
        
        // log the message (including the date/time values and current
        // process' id.
        logFile << ((std::string) buffer) << ", " << ((int) getpid()) << ", " << logMessage << std::endl;
    }
    // if the file is not open;
    else {
        // log the error message.
        fprintf(stderr, "Could not log the message %s, error %d occurred", logMessage.c_str(), errno);
    }
    
    // close the log file.
    logFile.close();
};

/**
 * Overloading method for the writeToLogFile method. The difference is
 * that, this method takes the name of a process and assigns that name
 * to the corresponding file of the logMessage which is actually a 
 * string format.
 * 
 * @param logMessage String format of the log message.
 * @param processName The name of the process that is coupled with the
 * log message.
 */
void Logger::writeToLogFile(std::string logMessage, std::string processName)
{
    // update the logMessage. it won't stay as a pattern.
    int messageLength = (int) logMessage.size() - 2 + (int) processName.size();
    char message[messageLength];
    sprintf(message, logMessage.c_str(), processName.c_str());
    
    // log the new message using the writeToLogFile (overloaded method) metod.
    Logger::writeToLogFile((std::string) message);
}
