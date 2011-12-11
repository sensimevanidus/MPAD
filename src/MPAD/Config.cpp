/**
 * File: Config.cpp
 * 
 * Source code file for the Config class.
 * 
 * @author Onur Yaman <onuryaman@gmail.com>
 * @version 0.1
 * @since 2011-10-28
 */

// include standard libraries.
#include <stdlib.h>

// include the stream related libraries.
#include <fstream>
#include <iostream>

// include the map library for the std::map class.
#include <map>

// include the string library for the std::string class.
#include <string>

// include the signature of the class.
#include "Config.h"
#include "Logger.h"

// use the standard namespace.
using namespace std;

// by default, the class is not instantiated.
bool Config::instanceFlag = false;
Config* Config::instance = NULL;

/**
 * Constructor of the Config class. Parses the file.
 */
Config::Config() {
    // parse the file as expected.
    this->parseFile();
}

/**
 * If the Config class was instantiated before; returns a pointer to
 * that instance. If not; instantiates the class, stores a pointer to
 * the instance and returns it.
 * 
 * @return Pointer to the singleton instance.
 */
Config* Config::getInstance()
{
    // if the class was not instantiated before;
    if (! Config::instanceFlag) {
        // instantiate it.
        Config::instance = new Config();
        
        // mark it as instantiated.
        Config::instanceFlag = true;
    }
    
    // return the singleton instance.
    return Config::instance;
}

/**
 * Given the section (main/child) of the configuration file and the
 * corresponding key of the setting, returns the value for that setting
 * in that section.
 * 
 * @param key The key of the configuration setting.
 * @param section The section in which the corresponding setting
 * resides.
 * @return The configuration setting value. 
 */
int Config::readValue(string key, string section) {
    return Config::getInstance()->configuration[section][key];
}

/**
 * Parses the configuration file and stores the corresponding settings
 * in the configuration property.
 */
void Config::parseFile() {
    // open the configuration file and make its content available for
    // processing.
    ifstream configFile("mpad.ini");
    
    // if the file was not opened properly;
    if (! configFile.is_open()) {
        // log the error message.
        Logger::writeToLogFile("ERROR: Could not open the configuration file! The application will terminate!");
        
        // terminate the program.
        exit(EXIT_FAILURE);
    }
    
    // the configuration file will be processed by being read line by line in a
    // loop. during the loop, each line must be stored in a local variable. so
    // create a local variable that will represent each line as a string.
    char line[50];
    
    // each configuration setting must be declared in a section of the
    // configuration file. the program must know that it's in a section or not.
    // a local variable will tell in which section the program is.
    char currentSection[50];
    
    // find the corresponding section of the configuration file.
    while (! configFile.eof()) {
        // read one line from the configuration file and store the line content
        // in the corresponding local variable.
        configFile.getline(line, 50, '\n');
        
        // if the line starts with the '[' character followed by a section name
        // and ends with the '[' character, we've hit a section start. check
        // for it. to do the checking, find the length of the line string first.
        int lineLength = strlen(line);

        // then, check whether the current line string contains a section name
        // or not.
        if ('[' == line[0] && ']' == line[lineLength - 1]) {
            // if the current line string contains a section name, update the
            // current section local variable to that section name.
            string tempStr((string) line);
            tempStr.copy(currentSection, lineLength - 2, 1);
            currentSection[lineLength - 2] = '\0';
            
            map<string, int> currentSectionConfigurationSettings;
            this->configuration[(string) currentSection] = currentSectionConfigurationSettings;
            
            // continue with the next iteration (because the current line is a
            // section identifier, it does not contain a configuration setting.
            continue;
        }

        // if the current section is the expected one, check for the
        // configuration settings.
        if (currentSection) {
            // parse the configuration setting.
            char* key = NULL;
            char* value = NULL;
            key = strtok(line, "=");
            value = strtok(NULL, "=");
            
            // if the configuration setting was parsed correctly;
            if (key && value) {
                // set the configuration setting in the instance variable.
                this->configuration[(string) currentSection][(string) this->trim(key)] = atoi(this->trim(value));
            }
        }
    }
    
    // file processing operation is finished now; so close the file.
    configFile.close();
}

/**
 * Trims the unnecessary white-spaces from the string and returns the
 * trimmed string.
 * 
 * @param rawString Un-trimmed string.
 * @return  Trimmed string.
 */
char* Config::trim(char* rawString) {
    // create local variable for looping and managing the locations of the 
    // actual chars in the raw string.
    int i, j, start, end;

    // trim the unnecessary characters to the left of the string.
    for (i=0; (rawString[i] != 0 && rawString[i] <= 32); ) {
        i++;
    }
    
    // mark the actual string's start index.
    start = i;

    // trim the unnecessary characters to the right of the string.
    for (i=0, j=0; rawString[i] != 0; i++) {
        j = ((rawString[i] <= 32) ? j+1 : 0);
    }
    
    // mark the actual string's end index.
    end = i-j;

    // trim the unnecessary characters.
    return (char*) ((string) rawString).substr(start, end-start).c_str();
}
