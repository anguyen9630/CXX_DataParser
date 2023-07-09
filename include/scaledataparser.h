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
        ScaleDataParser(std::string path, size_t baud);
        ~ScaleDataParser(){ std::cout << "Deleted data parser instance!"; };

        // Return attribute methods
        size_t          baud(){ return baudRate; };
        std::string     port(){ return serialPort; };
        
    private:
        // --------------- Private Attributes --------------- //
        // Configuration attributes
        size_t          baudRate;
        std::string     serialPort;

        // ----------------- Private Methods ---------------- //

};

#endif