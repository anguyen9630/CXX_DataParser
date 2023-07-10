#ifndef SERIALDRIVER_H
#define SERIALDRIVER_H

#include <iostream>
#include <cerrno>
#include <filesystem>
#include <string>

#include <fcntl.h> 
#include <termios.h> 
#include <unistd.h>

#include "utils.h"

class SerialDriver
{
    public:
        // --------------- Public Attributes ---------------- //

        // ----------------- Public Methods ----------------- //
        SerialDriver(const char* portPath, uint32_t baudRate);
        ~SerialDriver();
        std::string serialRead();
       
    private:
        // --------------- Private Attributes --------------- //
        int32_t     serialPort;
        termios     serialCfg, oldSerialCfg;
        
        // ----------------- Private Methods ---------------- //
        void        OpenSerialPort(const char* portPath);
        void        ConfigureSerialPort(uint32_t baudRate);
        speed_t     ToBaud(uint32_t baudRate);

};

#endif