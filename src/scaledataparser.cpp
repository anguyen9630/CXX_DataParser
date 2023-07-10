#include <scaledataparser.h>

/* 
 * The constructor instanciate a data parser object and
 * parses the provided data to prepare for serial connection.
 */
ScaleDataParser::ScaleDataParser(std::string path, int baud)
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

    serialDataList.clear();

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
 *  The function check if the data has been processed before collecting new data.
 *  When reading from serial, waits for the start character '/' being read collecting
 * the whole data. If the start character is not at the front, all data in front of 
 * it is purged.
 *  The function  will keep reading until the closing character '\' has been received.
 * Data after the character, if any, is also purged. 
 *  This function should be run on a separate thread.
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
        dataMutex.lock();
        // Add the new data to the back of the vector
        serialDataList.push_back(serialData);
        // Unlock the mutex and repeat for the next message.
        dataMutex.unlock();
    }
}

/*
 * TODO: Parses the collected data to JSON.
 */
void ScaleDataParser::ParseDataToJson()
{
    while (true)
    {
        // Lock mutex
        dataMutex.lock();
        // Get the size of the vector
        int listSize = serialDataList.size();
        // Unlock the mutex
        dataMutex.unlock();
        
        // When there is data in the list
        if (listSize)
        {  

            // Lock mutex
            dataMutex.lock();
            // Grab the first element
            std::string serialData = serialDataList.front();
            // Remove the first element
            serialDataList.erase(serialDataList.begin());
            // Unlock the mutex
            dataMutex.unlock();

            // Further processing is safe here.
            std::cout << serialData << std::endl;
        }
    }
    
}

void ScaleDataParser::RunParser()
{
    std::thread dataCollector(&ScaleDataParser::CollectDataFromSerial, this);
    std::thread jsonParser(&ScaleDataParser::ParseDataToJson, this);

    dataCollector.join();
    jsonParser.join();

}