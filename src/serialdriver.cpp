#include <serialdriver.h>

/* 
 * The constructor instanciate a serial driver object and
 * open the serial port provided.
 */
SerialDriver::SerialDriver(const char* SerialPort, size_t baudRate)
{
    
}

/* 
 * Using the long number recieved, convert to 
 * baud speed type. 
 * Note: only standard UNIX baud rates are used.
 */
speed_t SerialDriver::ToBaud(size_t baudRate)
{
    switch (baudRate)
    {
    case 0:
        return B0;
    case 50:
        return B50;
    case 75:
        return B75;
    case 110:
        return B110;
    case 134:
        return B134;
    case 150:
        return B150;
    case 200:
        return B200;
    case 300:
        return B300;
    case 600:
        return B1200;
    case 1200:
        return B1200;
    case 1800:
        return B1800;
    case 2400:
        return B2400;
    case 4800:
        return B4800;
    case 9600:
        return B9600;
    case 19200:
        return B19200;
    case 38400:
        return B38400;
    case 57600:
        return B57600;
    case 115200:
        return B115200;
    case 230400:
        return B230400;
    case 460800:
        return B460800;
    default:
        return NULL;
    }
}