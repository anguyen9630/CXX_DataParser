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
#include <ctime>

#include <signal.h>

#include <nlohmann/json.hpp>
#include "utils.h"
#include "serialdriver.h"

static bool stopProgram = false;

class ScaleDataParser
{
    public:
        // --------------- Public Attributes ---------------- //

        // ----------------- Public Methods ----------------- //
        ScaleDataParser(std::string path, int baud, int interval);
        ~ScaleDataParser();

        void                        RunParser();

        // Return attribute methods
        int                         Baud(){ return baudRate; };
        std::string                 Port(){ return serialPort; };
        
        
    private:
        // --------------- Private Attributes --------------- //
        // Configuration attributes
        int                         baudRate;
        int                         printInterval;
        std::string                 serialPort;
        
        // Serial data collection attributes
        SerialDriver*               serialDriver; 
        std::mutex                  rawDataMutex;
        std::vector<std::string>    serialDataList;

        // Json data attributes
        nlohmann::json              parsedData;
        std::mutex                  jsonDataMutex;
        bool                        dataReady;

        // ----------------- Private Methods ---------------- //
        void                        CollectDataFromSerial();
        
        nlohmann::json              ParseDataToJson(std::vector<std::string> serialData);
        std::vector<std::string>    SplitLines(std::string rawString);
        void                        ProcessData();

        void                        PrintData();
        
        static void                 TerminationHandler(int signum);
        
        
};

#endif