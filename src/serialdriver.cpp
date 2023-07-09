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
    // Get the default config
    if(tcgetattr(serialPort, &serialCfg) != 0) 
    {
        std::string errMsg = ErrorMsg(errno, "Failed to get default termios config. You should NOT be here...");
        throw errMsg;
    }

    serialCfg.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    serialCfg.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    serialCfg.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
    serialCfg.c_cflag |= CS8; // 8 bits per byte (most common)
    serialCfg.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    serialCfg.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    serialCfg.c_lflag &= ~ICANON; // Canonical mode 
    serialCfg.c_lflag &= ~(ECHO | ECHOE | ECHONL | ISIG); // Disable echo

    serialCfg.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    serialCfg.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

    serialCfg.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    serialCfg.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    speed_t baudSpeedT = ToBaud(baudRate);

    serialCfg.c_cc[VTIME] = 10;    
    serialCfg.c_cc[VMIN] = 0;

    // Set baud rate
    cfsetispeed(&serialCfg, baudSpeedT);
    cfsetospeed(&serialCfg, baudSpeedT);

    // Save config
    if (tcsetattr(serialPort, TCSANOW, &serialCfg) != 0) 
    {
        std::string errMsg = ErrorMsg(errno, "Failed to configure the serial port (How did this even happen...?)");
        throw errMsg;
    }
}

/*
 * Read from serial port. Per configuration, block until at least
 * 2 bytes (/\) has been received.
 */

std::string SerialDriver::serialRead()
{
    char dataBuffer[256];

    
    int ret = read(serialPort, &dataBuffer, sizeof(dataBuffer));

    

    if (ret)
    {
        std::cout << dataBuffer << std::endl;
    }
        
    
    return std::string(dataBuffer);
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