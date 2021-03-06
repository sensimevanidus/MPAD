/**
 * File: MainProcess.h
 * 
 * Header file for the MainProcess class.
 * 
 * @author Onur Yaman <onuryaman@gmail.com>
 * @version 0.2
 * @since 2011-11-28
 */

// make sure that the signature of the class is defined only once.
#ifndef MAINPROCESS_H
#define	MAINPROCESS_H

// include the string library for the std::string class.
#include <string>

// include the vector library.
#include <vector>

/**
 * Signature of the MainProcess class.
 * 
 * MainProcess class represents the main process that is responsible of creating
 * child processes (includes determining for how long they'll sleep, for how
 * many times they'll repeat the sleep/wake-up operations) and waiting for them
 * to terminate.
 * 
 * @since version 0.1
 */
class MainProcess
{
    // public attributes and methods of the class.
    public:
        
        /**
         * Constructor method of the class.
         */
        MainProcess();
        
        /**
         * Tries to create a new child process.
         * 
         * @see ChildProcess
         * @return The process id of the created child process. 
         */
        pid_t createChild(std::string rawString, std::string encryptedString, int algorithmId, int keyLength, int partitionNumber, int partitionSize);
        
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
        int getNumberOfChildren(int numberOfChildren);
        
        /**
         * Given the exact number of key length (e.g. 20-bits length), generates
         * a random key.
         * 
         * For the purpose of this project, maximum number of key size is
         * 128-bits.
         * 
         * @see ChildProcess
         * @param numberOfBits Exact value of key length.
         * @return The randomly generated key.
         */
        std::vector<unsigned char> generateRandomKey(int numberOfBits);
        
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
        std::string generateChildName(pid_t childPID);
};

// end of the class signature.
#endif	/* MAINPROCESS_H */
