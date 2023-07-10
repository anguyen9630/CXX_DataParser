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
#include <thread>
#include <mutex>

#include "utils.h"
#include "serialdriver.h"


class ScaleDataParser
{
    public:
        // --------------- Public Attributes ---------------- //

        // ----------------- Public Methods ----------------- //
        ScaleDataParser(std::string path, int baud);
        ~ScaleDataParser();
        
        void            CollectDataFromSerial();
        void            ParseDataToJson();
        void            RunParser();

        // Return attribute methods
        int             Baud(){ return baudRate; };
        std::string     Port(){ return serialPort; };

        
        
    private:
        // --------------- Private Attributes --------------- //
        // Configuration attributes
        int             baudRate;
        std::string     serialPort;
        
        // Serial data collection attributes
        SerialDriver*   serialDriver; 
        std::mutex      dataMutex;
        std::string     serialData;
        bool            newDataReady;
        bool            dataProcessed;


        // ----------------- Private Methods ---------------- //
        
};

#endif