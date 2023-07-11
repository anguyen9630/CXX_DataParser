#include <scaledataparser.h>

/* 
 * The constructor instanciate a data parser object and
 * parses the provided data to prepare for serial connection.
 */
ScaleDataParser::ScaleDataParser(std::string path, int baud, int interval)
{
    // Check baud rate for validity
    if (baud <= 0)
    {
        std::string errMsg = ErrorMsg(EINVAL, "Invalid baud rate. Input: " + std::to_string(baud));
        throw std::runtime_error(errMsg);
    }

    // Make sure that the interval time is above 0 
    if (interval <= 0)
    {
        std::string errMsg = ErrorMsg(EINVAL, "Interval must be greater than 0. Input: " + std::to_string(interval));
        throw std::runtime_error(errMsg);
    }
    // Make sure that interval is also lower or equal than 60
    else if (interval > 60)
    {
        std::string errMsg = ErrorMsg(EINVAL, "Interval must be smaller or equal to 60. Input: " + std::to_string(interval));
        throw std::runtime_error(errMsg);
    }

    // Initialise private attributes
    baudRate = baud;
    serialPort = path;
    printInterval = interval;
    serialDriver = new SerialDriver(path.c_str(), baud);
    dataReady = false;

    serialDataList.clear();
    parsedData.clear();

}

/* 
 * Destructor for ScaleDataParser delete all dynamically allocated
 * attributes then destroy the instance.
 */
ScaleDataParser::~ScaleDataParser()
{
    // Deleting object instances
    delete serialDriver;
    std::cout << "Deleted data parser instance!" << std::endl; 
}

/*
 * The function check if the data has been processed before collecting new data.
 * When reading from serial, waits for the start character '/' being read, before 
 * collecting the whole data. If the start character is not at the front,  
 * all data in front of it is purged.
 * The function  will keep reading until the closing character '\' has been received.
 * Data after the character, if any, is also purged. 
 * NOTE: This function should be run on a separate thread.
 */
void ScaleDataParser::CollectDataFromSerial()
{
    // Loop indefinitely until it is terminated
    while (true)
    {
        std::string serialData = "";
        bool dataAssembled = false;
        bool startCollection = false;
        std::string currentBuffer;
        
        // Collect data until a proper message has been collected
        while (!dataAssembled)
        {
            // Read from serial
            currentBuffer = serialDriver->serialRead();

            // Check for the "/" character
            int startPos = currentBuffer.find_first_of('/');
            // If found
            if (startPos != std::string::npos)
            {
                // Clear the data (Done here case of corrupted data and you get 2 '/')
                serialData.clear();

                // If there are data in front of the start character, purge
                currentBuffer.erase(0, startPos);
                startCollection = true;
            }
            
            // Collect the data and check for closing character
            if (startCollection)
            {
                // Check for the closing character
                int endPos = currentBuffer.find_first_of('\\');
                // If found
                if (endPos != std::string::npos)
                {
                    // Purge all data after the character
                    currentBuffer.erase(endPos+1);
                    dataAssembled = true;
                }
                
                // Append the buffer when all of the check has been completed
                serialData.append(currentBuffer);
            }
        }

        // Once a proper message has been collected, lock the mutex
        rawDataMutex.lock();
        // Add the new data to the back of the vector
        serialDataList.push_back(serialData);
        // Unlock the mutex and repeat for the next message.
        rawDataMutex.unlock();
    }
}

/*
 * Split lines of string using the '\n' character. Return
 * a vector by keep looping and collecting lines from the string.
 * If the string is empty then return an empty vector.
 */

std::vector<std::string> ScaleDataParser::SplitLines(std::string rawString)
{
    std::vector<std::string> lineList;
    std::string remainString = rawString;
    // Find the first line brake
    int newLinePos = remainString.find('\n');
    
    // If a line break found, loop until there are no longer any line break
    while(newLinePos != std::string::npos)
    {
        // Get the line as substring
        std::string line = remainString.substr(0, newLinePos);
        // Check if line is not empty 
        if (!line.empty())
        {
            // Remove carriage return
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
            // Add the line into the list
            lineList.push_back(line);
        }
        
        // Remove that line from the remaining string
        remainString.erase(0, newLinePos+1);
        // Find the next linebreak position
        newLinePos = remainString.find('\n');
    }

    // Add the remaining string into the vector
    if (!remainString.empty())
        lineList.push_back(remainString);

    return lineList;
}

/*
 * Parses the raw data from a vector of line strings to JSON. 
 * The function finds the colon character ':' for assignment.
 * If the colon character does not exist, the line is skipped.
 * The parser then grab the name, the unit and the value of the data.
 * And saves them in the JSON format.
 */
nlohmann::json ScaleDataParser::ParseDataToJson(std::vector<std::string> serialData)
{
    nlohmann::json data;
    int sum = 0;
    // Loop through each line
    for (std::string line : serialData)
    {
        // Delete all spaces from line
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

        // Find the separator (:)
        int seperatorPos = line.find(':');
        // Skip the line if not exist
        if (seperatorPos == std::string::npos) continue;

        // Note: Mass units are usually 1 or 2 characters
        // The last character is the unit
        
        std::string unit = "";
        unit.insert(unit.begin(), line.back());
        // If the character before the unit is not a digit (i.e. a char)
        if (!isdigit(line[line.size()-2]))
            // Insert to the front of the unit
            unit.insert(unit.begin(), line[line.size()-2]);

        // Make a substring for the name
        std::string name = line.substr(0, seperatorPos);
        // Get the integer value of data
        int value = atoi(line.substr(seperatorPos+1).c_str());
        
        // Assign the data using the name as key. The value and the unit is assigned to the corresponding keys
        data[name] = {{"VALUE", value}, {"UNIT", unit}};
        
        // Sum up all value that is not the TOTAL
        if (name != "TOTAL") sum += value;
        // If it is the total then compares the sum to the value for the VALID flag
        else
            data["VALID"] = sum == value;
    }

    
    return data;
}

/*
 * Process the collected raw data from serial.
 * Note: Should be run on a separate thread.
 */
void ScaleDataParser::ProcessData()
{
    while (true)
    {
        // Lock mutex
        rawDataMutex.lock();
        // Get the size of the vector
        int listSize = serialDataList.size();
        // Unlock the mutex
        rawDataMutex.unlock();
        
        // When there is data in the list
        if (listSize)
        {  

            // Lock mutex
            rawDataMutex.lock();
            // Grab the first element
            std::string serialData = serialDataList.front();
            // Remove the first element
            serialDataList.erase(serialDataList.begin());
            // Unlock the mutex
            rawDataMutex.unlock();

            // Further processing is safe here.
            std::vector<std::string> serialDataLines = SplitLines(serialData);
            // Parse data to JSON format
            nlohmann::json currentData = ParseDataToJson(serialDataLines);

            // Lock the JSON data mutex
            jsonDataMutex.lock();
            // Save the data
            parsedData = currentData;
            // Set that data is ready
            dataReady = true;
            // Unlock the JSON data mutex
            jsonDataMutex.unlock();
        }
    }
    
}

/*
 * Print the data every n seconds. The function get current time and divide it by 
 * the interval time. If modulo = 0 then print
 * Note: Should be run on a separate thread.
 */

void ScaleDataParser::PrintData()
{
    time_t previousTime;
    tm *currentTimeLocal;
    bool waitMessagePrinted = false;

    while(true)
    {
        // Lock the JSON data mutex
        jsonDataMutex.lock();
        bool dataAvailable = dataReady;
        jsonDataMutex.unlock();

        // If data is available
        if (dataAvailable)
        {
            // Get the current time
            time_t currentTime = time(NULL);

            // Make sure time is returned
            if (currentTime == -1)
            {
                std::string errMsg = ErrorMsg(errno, "Could not get time...");
                throw std::runtime_error(errMsg);
            }

            // Only print if it is a new timestamp
            if (currentTime == previousTime) continue;
            // Get the time in the tm struct
            currentTimeLocal = localtime(&currentTime);

            // Check to see if the second is divisible by the interval
            if(currentTimeLocal->tm_sec % printInterval == 0)
            {
                // Lock the JSON data mutex
                jsonDataMutex.lock();
                // Make a copy of the data
                nlohmann::json currentData = parsedData;
                // Unlock the JSON data mutex
                jsonDataMutex.unlock();

                // Convert tm to char* for printing
                char timeChar[20];
                int ret = std::strftime(timeChar, sizeof(timeChar), "Data at [%T]:", currentTimeLocal); 

                std::cout << timeChar << std::endl;

                for (auto& [key, val] : currentData.items())
                {
                    // For weight data, print the name and weight
                    if (key != "VALID")
                    {
                        std::string name = key;
                        int value = val["VALUE"];
                        std::string unit = val["UNIT"];
                        std::cout << name << ": " << std::to_string(value) + " " + unit << std::endl; 
                    }
                    // If it is the valid flag then print true or false
                    else
                    {
                        std::string name = key;
                        std::string value = val ? "TRUE" : "FALSE";
                        std::cout << name << ": " << value << std::endl;
                    }
                        
                }
                std::cout << "--------------------------------------------------------" << std::endl;
                std::cout << "Raw JSON:" << std::endl;
                std::cout << currentData << std::endl;
                std::cout << "********************************************************" << std::endl;
            }

            // Update the clock
            previousTime = currentTime;
        }

        // If data is not ready and message has not been printed
        else if (!waitMessagePrinted)
        {
            // Print a message
            std::cout << "Waiting for data..." << std::endl;
            waitMessagePrinted = true;
        }
        

    }
    // Delete the pointer
    delete currentTimeLocal;
}

void ScaleDataParser::RunParser()
{
    std::thread dataCollector(&ScaleDataParser::CollectDataFromSerial, this);
    std::thread jsonParser(&ScaleDataParser::ProcessData, this);
    std::thread dataLogger(&ScaleDataParser::PrintData, this);

    dataCollector.join();
    jsonParser.join();
    dataLogger.join();

}