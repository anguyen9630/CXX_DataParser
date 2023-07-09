#ifndef SCALEDATAPARSER_H
#define SCALEDATAPARSER_H

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <cerrno>
#include "utils.h"

class ScaleDataParser
{
    public:
        // --------------- Public Attributes ---------------- //

        // ----------------- Public Methods ----------------- //
        ScaleDataParser(const char* cfgFilePath);
        ~ScaleDataParser(){ std::cout << "Deleted data parser instance!"; };

        // Return attribute methods
        uint32_t        baud(){ return baudRate; };
        uint8_t         channels(){ return channelCount; };
        std::string     port(){ return serialPort; };
        
    private:
        // --------------- Private Attributes --------------- //
        // Configuration attributes
        uint32_t        baudRate = 2400;
        uint8_t         channelCount = 4;
        std::string     serialPort = "/dev/ttyUSB0";

        // ----------------- Private Methods ---------------- //
        void ParseConfig(const char* cfgFilePath);

};

#endif