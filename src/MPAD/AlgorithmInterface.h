/* 
 * File:   AlgorithmInterface.h
 * Author: onuryaman
 *
 * Created on December 12, 2011, 11:06 AM
 */

#ifndef ALGORITHMINTERFACE_H
#define	ALGORITHMINTERFACE_H

// include the string library for the std::string class.
#include <string.h>

class AlgorithmInterface
{
    public:
        std::string encrpyt(std::string rawString, const char* key, int keyLength);
        std::string decrypt(std::string encryptedString, const char* key, int keyLength);
};

#endif	/* ALGORITHMINTERFACE_H */

