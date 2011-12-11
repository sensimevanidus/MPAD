/**
 * File: Logger.h
 * 
 * Header file for the Logger class.
 * 
 * @author Onur Yaman <onuryaman@gmail.com>
 * @version 0.1
 * @since 2011-10-28
 */

// make sure that the signature of the class is defined only once.
#ifndef LOGGER_H
#define	LOGGER_H

// include the string library for the std::string class.
#include <string>

/**
 * Signature of the Logger class.
 * 
 * Logger class is a singleton class that manages the logging enhancements of
 * the project. Being a singleton class, it cannot be initialized explicitly
 * (the initialization phase is done implicitly).
 * 
 * The main process and all child processes share a common log file. The full
 * path of the log file is defined in the class.
 * 
 * @since version 0.1
 * @see MainProcess
 * @see ChildProcess
 */
class Logger
{
    // public attributes and methods of the class.
    public:
        
        /**
         * Full path of the shared log file.
         */
        static const char* logFilePath;
        
        /**
         * Writes a log message to the shared log file in a safe manner.
         * 
         * @param logMessage Event message that is going to be logged.
         */
        static void writeToLogFile(std::string logMessage);
        
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
        static void writeToLogFile(std::string logMessage, std::string processName);
        
        /**
         * If the Logger class was instantiated before; returns a pointer to
         * that instance. If not; instantiates the class, stores a pointer to
         * the instance and returns it.
         * 
         * @return Pointer to the singleton instance.
         */
        static Logger* getInstance();
        
    // private attributes and methods of the class.
    private:

        /**
         * Represents whether the class was instantiated before or not.
         */
        static bool instanceFlag;
        
        /**
         * The pointer to the single instance of the Logger class.
         */
        static Logger* instance;
};

// end of the class signature.
#endif	/* LOGGER_H */
