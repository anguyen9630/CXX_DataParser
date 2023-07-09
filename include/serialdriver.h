#ifndef SERIALDRIVER_H
#define SERIALDRIVER_H

#include <iostream>
#include <cerrno>
#include <filesystem>

#include <fcntl.h> 
#include <termios.h> 
#include <unistd.h>

class SerialDriver
{
    public:
        // --------------- Public Attributes ---------------- //

        // ----------------- Public Methods ----------------- //
        SerialDriver(const char* SerialPort, size_t baudRate);
        ~SerialDriver(){ std::cout << "Deleted serial driver instance!"; };

        speed_t ToBaud(size_t baudRate);

        
       
    private:
        // --------------- Private Attributes --------------- //
        size_t serialPort;
        
        // ----------------- Private Methods ---------------- //


};

#endif