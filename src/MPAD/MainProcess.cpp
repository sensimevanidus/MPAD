/**
 * File: MainProcess.cpp
 * 
 * Source code file for the MainProcess class.
 * 
 * @author Onur Yaman <onuryaman@gmail.com>
 * @version 0.3
 * @since 2011-11-22
 */

// include standard libraries.
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// include the time library for date/time related functions.
#include <time.h>

// include the string library for the std::string class.
#include <string.h>

// include the stream related libraries.
#include <iosfwd>
#include <sstream>
#include <iostream>

// include the errno library for the errno variable.
#include <errno.h>

// include the vector library.
#include <vector>

// include the signature of the class.
#include "MainProcess.h"

// include the Config class signature.
#include "Config.h"

// include the Logger class signature.
#include "Logger.h"
#include "AlgorithmDES.h"

/**
 * Main entry of the application.
 * 
 * @return The exit status of the application.
 */
int main()
{
    // instantiate the MainProcess class.
    new MainProcess;
    
    // terminate the application.
    exit(EXIT_SUCCESS);
}

/**
 * Constructor method of the class. Creates child processes and waits for each
 * of the created child process. When they all terminate, it returns.
 */
MainProcess::MainProcess()
{
    // log the main process' start event.
    Logger::writeToLogFile("Main process started!");
    
    // calculate the number of child processes to be created.
    int processCount = this->getNumberOfChildren(Config::readValue("numberOfChildren", "main"));
    
    // store the process ids of all child processes. it will be used for waiting
    // for their termination.
    int processIds[processCount];

    // create a random seed.
    srand((unsigned) time(NULL));
    // create the key.
    std::vector<unsigned char> key(56, '0');
    // generate the key randomly.

    for (int i = 40; i < 56; i++) {
        int ran = (((((rand()) % 65536) + 1) >> i) & 1);
        if (0 == ran) {
            key.at(i) = '0';
        } else {
            key.at(i) = '1';
        }
    }

    // generate the random key.
    Logger::writeToLogFile("Key is: %s", (const char*) key.data());
    
    // -------------------------------------------------------------------------
    // @todo
    AlgorithmDES algoDES;
    std::string rawString("This is the raw string!");
    std::string encryptedString;
    encryptedString = algoDES.encrpyt(rawString, (const char*) key.data(), 56);
    
    // create child processes.
    for (int i = 0; i < processCount; i++) {
        // store the child process id in the local variable.
        processIds[i] = this->createChild(rawString, encryptedString, 1, 56, i, processCount);
    }
    // -------------------------------------------------------------------------
    
    // wait for each child to exit.
    for (int i = 0; i < processCount; i++) {
        // create a local variable for the exit status of the child process.
        int status;
        
        // wait for the ith child to terminate.
        while (-1 == waitpid(processIds[i], &status, 0));
        
        // log the child process' terminate event.
        Logger::writeToLogFile("Child process %s is terminated!",
            this->generateChildName(processIds[i])
        );
        
        if (WIFEXITED(status)) {
            // if the key is found;
            if (2 == WEXITSTATUS(status)) {
                Logger::writeToLogFile("All child processes will be terminated now!");

                // kill all child processes.
                for (int i = 0; i < processCount; i++) {
                    kill(processIds[i], SIGTERM);
                }
            }
        }
    }
    
    // log the main process' terminate event.
    Logger::writeToLogFile("Main process ended!");
}

/**
 * Tries to create a new child process. If it can't create a child, logs an
 * error message.
 * 
 * @see ChildProcess
 * @return The process id of the created child process. 
 */
pid_t MainProcess::createChild(std::string rawString, std::string encryptedString, int algorithmId, int keyLength, int partitionNumber, int partitionSize)
{
    // log the child process' creation event.
    Logger::writeToLogFile("A new child will be created!");
    
    // fork the process (create the child).
    pid_t pid = fork();

    // if this is the child process;
    if (0 == pid) {
        // generate a name for the child process.
        std::string childName = this->generateChildName((int) getpid()).c_str();

        std::stringstream ss1, ss2, ss3, ss4;
        ss1 << algorithmId;
        ss2 << keyLength;
        ss3 << partitionNumber;
        ss4 << partitionSize;
        
        // then instantiate the ChildProcess class.
        execl("./child_p", "child_p", childName.c_str(), rawString.c_str(), encryptedString.c_str(), ss1.str().c_str(), ss2.str().c_str(), ss3.str().c_str(), ss4.str().c_str(), NULL);
        /*
        // if an error occurred;
        if (0 != status) {
            // create a local variable for the error number.
            char errorBuffer[5];

            // write the error number to the buffer.
            sprintf(errorBuffer, "%d", errno);
        
            // log the corresponding error message (including the error number).
            Logger::writeToLogFile("ERROR: An error occurred, error %s!", errorBuffer);
        }
        */
    }
    
    // if this is the parent process;
    else if (0 < pid) {
        // log the child process' created event.
        Logger::writeToLogFile("Created %s!", this->generateChildName(pid));
    }
    
    // if the child could not be created;
    else {
        // create a local variable for the error number.
        char errorBuffer[5];
        
        // write the error number to the buffer.
        sprintf(errorBuffer, "%d", errno);
        
        // log the corresponding error message (including the error number).
        Logger::writeToLogFile("ERROR: Can't fork, error %s!", errorBuffer);
    }
    
    // return the process id of the child process.
    return pid;
}

/**
 * Given the number of child processes to be created from the
 * configuration file, checks the process quota of the current user,
 * regulates the number and returns the new number.
 * 
 * @see Config
 * @param numberOfChildren Number of child processes to be created that
 * is defined in the configuration file.
 * @return Regulated number of child processes to be created.
 */
int MainProcess::getNumberOfChildren(int numberOfChildren)
{
    // create a local variable for storing a pointer to the file pipe that is
    // going to be used to fetch the output of the shell commands.
    FILE* pipe;
    
    // create a local variable for storing the shell command output.
    char line[256];
    
    // find the process quota of the current user for the current running
    // environment (the "r" parameter is there for making it possible to read
    // command results from stdin, the output of the quota check command).
    const char* quotaCheckCommand = "ulimit -u";
    if (! (pipe = (FILE*) popen(quotaCheckCommand, "r"))) {
        Logger::writeToLogFile("ERROR: Could not determine the process quota for the current user. Main process exits!");
        exit(EXIT_FAILURE);
    }
    
    // there'll be just a single line of output; so we don't need to run the
    // fgets command in a loop.
    fgets(line, sizeof(line), pipe);
    
    // we have the output of the command, it's wise to close the pipe now.
    pclose(pipe);
    
    // create a local variable for storing the process quota for the current
    // user.
    int processQuota;
    
    // parse the output of the process quota command, calculate the
    // corresponding value and assign it to the previously defined local
    // variable (for string trimming, the public method of the Config class,
    // trim can be used).
    processQuota = atoi(Config::getInstance()->trim(line));
    
    // find the number of running processes (again, the "r" parameter is there
    // for making it possible to read command results from stdin, the output of
    // the number of running processes calculation command).
    const char* numberOfRunningProcessesCommand = "ps -ef|wc -l";
    if (! (pipe = (FILE*) popen(numberOfRunningProcessesCommand, "r"))) {
        Logger::writeToLogFile("ERROR: Could not determine the number of running processes of the current user. Main process exits!");
        exit(EXIT_FAILURE);
    }
    
    // there'll be just a single line of output; so we don't need to run the
    // fgets command in a loop.
    fgets(line, sizeof(line), pipe);
    
    // we have the output of the command, it's wise to close the pipe now.
    pclose(pipe);
    
    // create a local variable for storing the number of running processes of
    // the current user.
    int numberOfRunningProcesses;
    
    // parse the output of the number of running processes command, calculate
    // the corresponding value and assign it to the previously defined local
    // variable (for string trimming, the public method of the Config class,
    // trim can be used).
    numberOfRunningProcesses = atoi(Config::getInstance()->trim(line));
    
    // define an offset that will determine the boundaries for the processes to
    // be created.
    int offset = 10;
    
    // calculate the maximum value for the number of child processes to be
    // created.
    int maxNumberOfChildProcesses = processQuota - numberOfRunningProcesses - offset;
    
    // check whether the configuration setting exceeds the possible limit or
    // not.
    if (numberOfChildren > maxNumberOfChildProcesses) {
        // the configuration setting value exceed the possible limit; so log
        // that the configuration setting will be discarded and instead, the
        // maximum number will be used.
        Logger::writeToLogFile("The number of child processes to be created defined in the configuration file exceeds the process quota for the current user; so the configuration setting is discarded. Instead, the maximum value will be used.");
        numberOfChildren = maxNumberOfChildProcesses;
    }
        
    // return the number of child processes to be created.
    return numberOfChildren;
}

/**
 * Given an integer that represents the length of the key to be produced,
 * generates a random key and returns it.
 * 
 * Note that the size of the key length can be 128 at maximum.
 * 
 * @see ChildProcess
 * @param numberOfBits Length of the key to be generated.
 * @return The randomly generated ley.
 */
std::vector<unsigned char> generateRandomKey(int numberOfBits)
{
    // create the key.
    std::vector<unsigned char> key(numberOfBits, '0');

    // generate the key randomly.
    for (int i = 0; i < numberOfBits; i++) {
        key.at(i) = (unsigned char) (rand() % 2);
    }
    
    // return the key.
    return key;
}

/**
 * Given the process id of a child process, generates a unique name for
 * that child. The name will be used to uniquely identify a child
 * process in the log file.
 * 
 * @see ChildProcess
 * @see Logger
 * @param childPID Process id of the child process.
 * @return Uniquely created name of the child process.
 */
std::string MainProcess::generateChildName(int childPID)
{
    // create a local variable for the child process name.
    char childName[50];
    
    // generate the child name.
    sprintf(childName, "CHILD-%d", childPID);
    
    // cast the child process name as s string.
    std::string strChildName(childName);
    
    // return the child process name.
    return strChildName;
}