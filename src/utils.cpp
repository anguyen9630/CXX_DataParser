#include <utils.h>

/*
 * Function to construct error messages.
 */

std::string ErrorMsg(int8_t errorNo, std::string msg)
{
    return "Error " + std::to_string(errorNo) + ": " + std::strerror(errorNo) + " | [" + msg +"]";
}