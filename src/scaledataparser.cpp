#include <scaledataparser.h>

/* 
 * The constructor instanciate a data parser object and
 * parses the information from the config file, if provided,
 * to prepare for the serial connection.
 */
ScaleDataParser::ScaleDataParser(const char* cfgFilePath = NULL)
{
    
    // If a path is provided then parse the data, else use default
    if (cfgFilePath) ParseConfig(cfgFilePath);

    else
        std::cout << "Initalising with default configuration [Port: /dev/ttyUSB0 | Baud: 2400 | Channels: 4]..." << std::endl;

}

void ScaleDataParser::ParseConfig(const char* cfgFilePath)
{
    bool portFound = false, baudFound = false, chCountFound = false;

        // Instanciate an ifstream object and open the config file using the path
        std::ifstream cfgFileStream (cfgFilePath);

        // Check if file is open
        if(cfgFileStream.is_open())
        {

            std::string line;
            // Read the config file line by line
            while (std::getline(cfgFileStream, line))
            {
                // Remove all empty spaces from the line
                line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

                // Skip if line is empty or is a comment
                if (line.empty() || line[0] == '#') continue;

                // If the assignment is located at the end of the line (i.e. no value), skip the line
                if (line.back() == '=') continue;
                
                // Find the position of the '=' sign
                size_t assignIndx = line.find('=');
                
                // If the '=' not in the line then continue
                if (assignIndx == std::string::npos) continue;

                // Get the name portion of the parameter
                std::string paramName = line.substr(0, assignIndx);

                // Get the value portion of the parameter
                std::string paramVal = line.substr(assignIndx+1);

                // Assign the value based on name
                if (paramName == "dev")
                {
                    serialPort = paramVal;
                    portFound = true;
                } 

                else if (paramName == "baud") 
                {
                    // Convert string to int
                    int baudCfg = atoi(paramVal.c_str());

                    // Make sure baud rate is above 0
                    if (baudCfg < 0)
                    {   
                        std::string errMsg = ErrorMsg(EINVAL, "Invalid baud rate: " + std::to_string(baudCfg));
                        throw std::runtime_error(errMsg);
                    }

                    // Assign if ok
                    baudRate = baudCfg;
                    baudFound = true;
                }

                else if (paramName == "num-channels") {
                    
                    // Convert string to int
                    channelCount = atoi(paramVal.c_str());
                    
                    // Make sure that only 4 or 6 is set for the channel count
                    if (!(channelCount == 4 || channelCount == 6))
                    {
                        std::string errMsg = ErrorMsg(EINVAL, "Parser only support 4 or 6 channels. Config: " + std::to_string(channelCount));
                        throw std::runtime_error(errMsg);
                    }

                    chCountFound = true;
                }
            }
        }
        // If file cannot be opened
        else
        {
            std::string errMsg = ErrorMsg(EIO, "Could not open the config file: " + std::string(cfgFilePath));
            
            throw std::runtime_error(errMsg);
        }
            
        if (!portFound)
            std::cout << "Warning: Serial port not found in config file. Using default: " << serialPort << std::endl;
        if (!baudFound)
            std::cout << "Warning: Baud rate not found in config file. Using default: " << baudRate << std::endl;
        if (!chCountFound)
            std::cout << "Warning: Channel count not found in config file. Using default: " << channelCount << std::endl;
}