/**
 * File: ChildProcess.h
 * 
 * Header file for the ChildProcess class.
 * 
 * @author Onur Yaman <onuryaman@gmail.com>
 * @version 0.1
 * @since 2011-10-28
 */

// make sure that the signature of the class is defined only once.
#ifndef CHILDPROCESS_H
#define	CHILDPROCESS_H

// include the string library for the std::string class.
#include <string>

/**
 * Signature of the ChildProcess class.
 * 
 * ChildProcess class represents a child process that is responsible of sleeping
 * and then waking up as described in the design specification document.
 * 
 * @since version 0.1
 */
class ChildProcess
{
    // public attributes and methods of the class.
    public:
        
        /**
         * Constructor method of the class.
         * 
         * @param name The name of the child process that is given by the main
         * process.
         */
        ChildProcess(std::string name, std::string rawString, std::string encryptedString, int algorithmId, int keyLength, int partitionNumber, int partitionSize);
    
    // private attributes and methods of the class.
    private:
        
        /**
         * The name of the child process that is given by the main process.
         */
        std::string name;
};

// end of the class signature.
#endif	/* CHILDPROCESS_H */
