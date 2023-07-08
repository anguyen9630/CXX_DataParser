#include <scaledataparser.h>

void PrintHelp()
{
    std::cout << "C++ Data Parser for Pacific Scale - MT-Data Trial" << std::endl;
    std::cout << "Usage: scaleparser [-h|--help] [-c|--config <path>]" << std:: endl;
}


int main(int argc, char *argv[])
{
    char* configFilePath = NULL;

    // Loop through all the command line arguments
    for (size_t indx = 1; indx < argc; indx++)
    {
        std::string currentArg = std::string(argv[indx]);
        
        if (currentArg == "-h" || currentArg == "--help")
        {
            PrintHelp();
            return 0;
        }

        // Check for config files
        else if (currentArg == "-c" || currentArg == "--config")
        {
            // Made sure that a config file actually has been provided.
            if (indx + 1 <= argc-1)
                configFilePath = argv[indx+1];
            else
            {
                std::cout << "Error: You did not provide a config file." << std::endl;
                return -1;
            }
        }
    }

    ScaleDataParser *parser = new ScaleDataParser(configFilePath);
    std::cout << "Initalised parser! Serial port: " << parser->port();
    std::cout << " | Baud rate: " << parser->baud();
    std::cout << " | No. Channels: " << parser->channels() << std::endl;
    return 0;
};