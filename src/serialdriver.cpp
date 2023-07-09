#include <serialdriver.h>

/* 
 * The constructor instanciate a serial driver object and
 * open the serial port provided and configure the port.
 */
SerialDriver::SerialDriver(const char* portPath, uint32_t baudRate)
{
    // Open the serial port
    OpenSerialPort(portPath);

    // Configure the serial port
    ConfigureSerialPort(baudRate);
}

SerialDriver::~SerialDriver()
{
    // Close the serial port
    close(serialPort);

    std::cout << "Deleted serial driver instance!" << std::endl;
}

/*
 * The function open the provided serial port provided and check 
 * for port validity.
 */
void SerialDriver::OpenSerialPort(const char* portPath)
{
    // Open the serial port in read only mode
    serialPort = open(portPath, O_RDONLY);

    // If serial port return is lower than 0 then error occured
    if (serialPort < 0) {
        std::string errMsg = ErrorMsg(errno, "Failed to open the serial port: " + std::string(portPath));
        throw errMsg;
    }
}

/*
 * The function open the provided serial port provided and check 
 * for port validity. The program is hardcored for:
 * - Non-parity
 * - 1 stop bit
 * - 8-bits/byte
 * - No RTS/CTS
 * - No Canonical (All lines at once)
 */
void SerialDriver::ConfigureSerialPort(uint32_t baudRate)
{
    // Disable parity
    serialCfg.c_cflag &= ~PARENB;
    // Single stop bit
    serialCfg.c_cflag &= ~CSTOPB;
    // 8-bits per byte
    serialCfg.c_cflag |= CS8;
    // Disable RTS/CTS
    serialCfg.c_cflag &= ~CRTSCTS;
    // Turn on Read and Local
    serialCfg.c_cflag |= CREAD | CLOCAL;
    // Disable canonical
    serialCfg.c_lflag &= ~ICANON;
    // Disable echo
    serialCfg.c_lflag &= ~ECHO;
    // No need for INTR, QUIT, SUSP so disable
    serialCfg.c_lflag &= ~ISIG;
    // Disable sw flow control
    serialCfg.c_iflag &= ~(IXON | IXOFF | IXANY);
    // Only grab rawdata, no special handling
    serialCfg.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
    
    // Return when at least 1 byte of data was received.
    serialCfg.c_cc[VTIME] = 0;   
    serialCfg.c_cc[VMIN] = 1;

    // Set baud rate
    cfsetspeed(&serialCfg, ToBaud(baudRate));


    // Save config
    if (tcsetattr(serialPort, TCSANOW, &serialCfg) != 0) {
        std::string errMsg = ErrorMsg(errno, "Failed to configure the serial port (How did this even happen...?)");
        throw errMsg;
    }
}


/* 
 * Using the long number recieved, convert to baud speed type. 
 * Note: only standard UNIX baud rates are used.
 */
speed_t SerialDriver::ToBaud(uint32_t baudRate)
{
    switch (baudRate)
    {
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
        std::string errMsg = ErrorMsg(EINVAL, "This program only support standard UNIX baud rates. Config: " + std::to_string(baudRate));
        throw errMsg;
    }
}