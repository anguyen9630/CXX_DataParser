#ifndef PARSERUTILS_H
#define PARSERUTILS_H

#include <string>
#include <cstring>
#include <iostream>
#include <cstring>
#include <mutex>

#include <signal.h>

extern volatile bool    terminateProgram;
extern std::mutex       termFlagMutex;

std::string ErrorMsg(int8_t errorNo, std::string msg);
void signalHandler(int signum);

void setupSignalHandling();

#endif