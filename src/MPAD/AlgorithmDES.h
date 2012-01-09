/* 
 * File:   AlgorithmDES.h
 * Author: onuryaman
 *
 * Created on December 12, 2011, 11:15 AM
 */

#ifndef ALGORITHMDES_H
#define	ALGORITHMDES_H

// include the AlgorithmInterface header file.
#include "AlgorithmInterface.h";

class AlgorithmDES : public AlgorithmInterface
{
    public:
        std::string encrpyt(std::string rawString, const char* key, int keyLength);
        std::string decrypt(std::string encryptedString, const char* key, int keyLength);
};

#endif	/* ALGORITHMDES_H */