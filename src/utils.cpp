#include <utils.h>
volatile bool   terminateProgram = false;
std::mutex      termFlagMutex;

/*
 * Function to construct error messages.
 */

std::string ErrorMsg(int8_t errorNo, std::string msg)
{
    return "Error " + std::to_string(errorNo) + ": " + std::strerror(errorNo) + " | [" + msg +"]";
}

void signalHandler(int signum)
{
    std::cout << std::endl << "Termination request received: " << std::to_string(signum) << std::endl;
    termFlagMutex.lock();
    terminateProgram = true;
    termFlagMutex.unlock();
}

void setupSignalHandling()
{
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGPIPE, signalHandler);
}