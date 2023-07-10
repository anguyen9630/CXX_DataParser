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
#include <vector>
#include <nlohmann/json.hpp>

#include "utils.h"
#include "serialdriver.h"


class ScaleDataParser
{
    public:
        // --------------- Public Attributes ---------------- //

        // ----------------- Public Methods ----------------- //
        ScaleDataParser(std::string path, int baud);
        ~ScaleDataParser();

        void                        RunParser();

        // Return attribute methods
        int                         Baud(){ return baudRate; };
        std::string                 Port(){ return serialPort; };

        
        
    private:
        // --------------- Private Attributes --------------- //
        // Configuration attributes
        int                         baudRate;
        std::string                 serialPort;
        
        // Serial data collection attributes
        SerialDriver*               serialDriver; 
        std::mutex                  rawDataMutex;
        std::vector<std::string>    serialDataList;

        // Json data attribute
        nlohmann::json              parsedData;
        std::mutex                  jsonDataMutex;

        // ----------------- Private Methods ---------------- //
        void                        CollectDataFromSerial();
        nlohmann::json              ParseDataToJson(std::vector<std::string> serialData);
        void                        ProcessData();
        std::vector<std::string>    SplitLines(std::string rawString);
        
};

#endif