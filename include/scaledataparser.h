#ifndef SCALEDATAPARSER_H
#define SCALEDATAPARSER_H

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <filesystem>

class ScaleDataParser
{
    public:
        // --------------- Public Attributes ---------------- //

        // ----------------- Public Methods ----------------- //
        ScaleDataParser(const char* cfgFilePath);
        ~ScaleDataParser(){ std::cout << "Deleted data parser instance!"; };

        // Return attribute methods
        int             baud(){ return baudRate; };
        int             channels(){ return channelCount; };
        std::string     port(){ return serialPort; };
        
    private:
        // --------------- Private Attributes --------------- //
        // Configuration attributes
        size_t          baudRate = 2400;
        size_t          channelCount = 4;
        std::string     serialPort = "/dev/ttyUSB0";

        // ----------------- Private Methods ---------------- //


};

#endif