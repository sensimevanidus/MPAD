/**
 * File: Config.h
 * 
 * Header file for the Config class.
 * 
 * @author Onur Yaman <onuryaman@gmail.com>
 * @version 0.1
 * @since 2011-10-28
 */

// make sure that the signature of the class is defined only once.
#ifndef CONFIG_H
#define	CONFIG_H

// include the map library for the std::map class.
#include <map>

// include the string library for the std::string class.
#include <string>

/**
 * Signature of the Config class.
 * 
 * Config class is a singleton class that defines the configuration settings of
 * the project. Being a singleton class, it cannot be initialized explicitly
 * (the initialization phase is done implicitly).
 * 
 * The main process uses the configuration file for determining the number of
 * child processes it will create, the maximum number of seconds a child can
 * sleep and the maximum number of times the sleep/wake-up operations repeat.
 * 
 * The configuration file shall be created regarding to the INI standards. The
 * full path of the log file is defined in the class. For INI standards, please
 * refer to {@see http://en.wikipedia.org/wiki/INI_file}.
 * 
 * @since version 0.1
 * @see MainProcess
 */
class Config
{
    // public attributes and methods of the class.
    public:
        
        /**
         * Full path of the configuration file.
         */
        static const char* configFilePath;
        
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
        static int readValue(std::string key, std::string section);
        
        /**
         * If the Config class was instantiated before; returns a pointer to
         * that instance. If not; instantiates the class, stores a pointer to
         * the instance and returns it.
         * 
         * @return Pointer to the singleton instance.
         */
        static Config* getInstance();
        
        /**
         * Trims the unnecessary white-spaces from the string and returns the
         * trimmed string.
         * 
         * @param rawString Un-trimmed string.
         * @return  Trimmed string.
         */
        char* trim(char* rawString);

    // private attributes and methods of the class.
    private:
        
        /**
         * The hashmap of the configuration settings. The configuration file is
         * parsed and corresponding settings are stored in this hashmap.
         */
        std::map<std::string, std::map<std::string, int> > configuration;
        
        /**
         * Constructor of the Config class.
         */
        Config();
        
        /**
         * Destructor of the Config class.
         */
        ~Config()
        {
            instanceFlag = false;
        }
        
        /**
         * Parses the configuration file and stores the corresponding settings
         * in the configuration property.
         */
        void parseFile();

        /**
         * Represents whether the class was instantiated before or not.
         */
        static bool instanceFlag;
        
        /**
         * The pointer to the single instance of the Config class.
         */
        static Config* instance;
};

// end of the class signature.
#endif	/* CONFIG_H */
