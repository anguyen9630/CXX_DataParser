#include <scaledataparser.h>

void PrintHelp()
{
    std::cout << "C++ Data Parser for Pacific Scale - MT-Data Trial" << std::endl;
    std::cout << "Usage: scaleparser [-h|--help]" << std::endl;
    std::cout << "                   [-p|--port <path>] [-b|--baud <number>]" << std::endl;
}


int main(int argc, char *argv[])
{
    std::string portPath = "";
    size_t baudRate = 0;
    
    // If no argument was given, print help
    if (argc < 2)
    {
        PrintHelp();
        return 0;
    }

    // Loop through all the command line arguments
    for (size_t indx = 1; indx < argc; indx++)
    {
        std::string currentArg = std::string(argv[indx]);
        
        if (currentArg == "-h" || currentArg == "--help")
        {
            PrintHelp();
            return 0;
        }

        // Check for serial port flag
        else if (currentArg == "-p" || currentArg == "--port")
        {
            // Made sure that a path was actually provided.
            if (indx + 1 <= argc-1)
                portPath = std::string(argv[indx+1]);
            else
            {
                std::cout << "Error: You did not provide a path to the serial port." << std::endl;
                PrintHelp();
                return -1;
            }
        }

        // Check for baud rate flag
        else if (currentArg == "-b" || currentArg == "--baud")
        {
            // Made sure that a baud rate was actually provided.
            if (indx + 1 <= argc-1)
                baudRate = atoi(argv[indx+1]);
            else
            {
                std::cout << "Error: You did not provide a baud rate." << std::endl;
                PrintHelp();
                return -1;
            }
        }
    }

    // Make sure that enough arguments are provided
    if (portPath.empty())
    {
        std::cout << "Error: You did not provide a path to the serial port." << std::endl;
        PrintHelp();
        return -1;
    }
    else if (!baudRate)
    {
        std::cout << "Error: You did not provide a baud rate." << std::endl;
        PrintHelp();
        return -1;
    }

    try
    {
        ScaleDataParser parser(portPath, baudRate);
        std::cout << "Initalised parser! Serial port: " << parser.port();
        std::cout << " | Baud rate: " << parser.baud() << std::endl;
        
        return 0;
    }
    
    catch(std::runtime_error e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    
};