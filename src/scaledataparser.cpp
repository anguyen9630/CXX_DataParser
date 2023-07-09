#include <scaledataparser.h>

/* 
 * The constructor instanciate a data parser object and
 * parses the provided data to prepare for serial connection.
 */
ScaleDataParser::ScaleDataParser(std::string path, size_t baud)
{
    // Check baud rate for validity
    if (baud < 0)
    {
        std::string errMsg = ErrorMsg(EINVAL, "Invalid baud rate: " + std::to_string(baud));
        throw std::runtime_error(errMsg);
    }

    // Initialise private attributes
    baudRate = baud;
    serialPort = path;
    serialDriver = new SerialDriver(path.c_str(), baud);

}

ScaleDataParser::~ScaleDataParser()
{
    // Deleting object instances
    delete serialDriver;
    std::cout << "Deleted data parser instance!" << std::endl; 
}