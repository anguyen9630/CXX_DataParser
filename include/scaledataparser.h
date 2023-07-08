#ifndef SCALEDATAPARSER_H
#define SCALEDATAPARSER_H

#include <iostream>
#include <cstdint>
#include <string>

class ScaleDataParser
{
    public:
        ScaleDataParser();
        ~ScaleDataParser();
        
    private:
        // Configuration attributes
        uint32_t        baudRate;
        uint16_t        channelCount;
        std::string     serialPort;

};

#endif