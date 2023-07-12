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
    // Reset the old config
    tcsetattr(serialPort, TCSANOW, &oldSerialCfg);
    // Close the serial port
    close(serialPort);

    std::cout << "Deleted serial driver instance." << std::endl;
}

/*
 * The function open the provided serial port provided and check 
 * for port validity.
 */
void SerialDriver::OpenSerialPort(const char* portPath)
{
    // Open the serial port in Read & Write with no terminal control and non block
    serialPort = open(portPath, O_RDWR | O_NOCTTY | O_NONBLOCK);

    // If serial port return is lower than 0 then error occured
    if (serialPort < 0) {
        std::string errMsg = ErrorMsg(errno, "Failed to open the serial port: " + std::string(portPath));
        throw std::runtime_error(errMsg);
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
    // Get the default config
    if(tcgetattr(serialPort, &oldSerialCfg) != 0) 
    {
        std::string errMsg = ErrorMsg(errno, "Failed to get default termios config. You should NOT be here...");
        throw std::runtime_error(errMsg);
    }
    // Set everything to zero to only enable what is needed
    bzero(&serialCfg, sizeof(serialCfg));
    // Input is UTF8
    serialCfg.c_iflag = IUTF8;
    // No flag for output (likely will not even be used)
    serialCfg.c_oflag = 0;
    // 8-bits byte, enable read and ingore modem
    serialCfg.c_cflag = CS8 | CREAD | CLOCAL;
    // No flag for lflag (no canonical, no echo, etc.)
    serialCfg.c_lflag = 0;

    // Return as soon as port is ready
    serialCfg.c_cc[VTIME] = 0;    
    serialCfg.c_cc[VMIN] = 0;

    // Set baud rate
    cfsetispeed(&serialCfg, ToBaud(baudRate));
    cfsetospeed(&serialCfg, ToBaud(baudRate));
    
    // Flush the serial port
    if (tcflush(serialPort, TCIOFLUSH) != 0) 
    {
        std::string errMsg = ErrorMsg(errno, "Failed to flush serial port...");
        throw std::runtime_error(errMsg);
    }
    // Save config
    if (tcsetattr(serialPort, TCSANOW, &serialCfg) != 0) 
    {
        std::string errMsg = ErrorMsg(errno, "Failed to configure the serial port (How did this even happen...?)");
        throw std::runtime_error(errMsg);
    }
}

/*
 * Read from serial port and check from error
 */
std::string SerialDriver::serialRead()
{  
    // Declare a buffer
    char dataBuffer[256];
    // Declare a size variable to handle return
    int receiveSize = 0;
    
    // While the buffer does not receive any information
    while (!receiveSize && !terminateProgram)
    {
        // Read from serial port
        receiveSize = read(serialPort, &dataBuffer, sizeof(dataBuffer));
        
        // If read failed, throw an error
        if (receiveSize < 0) 
        {
            std::string errMsg = ErrorMsg(errno, "Reading from serial port failed!");
            throw std::runtime_error(errMsg);
        }
    }
    // Set the last part as 0
    dataBuffer[receiveSize] = 0;
    
    return std::string(dataBuffer);
}


/* 
 * Using the long number recieved, convert to baud speed type. 
 * Custom baudrate is supported by directly using the integer to setup.
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
    //case 460800:
        //return B460800;
    default:
        return baudRate;
    }
}