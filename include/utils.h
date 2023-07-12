#ifndef PARSERUTILS_H
#define PARSERUTILS_H

#include <string>
#include <cstring>
#include <iostream>
#include <cstring>

#include <signal.h>

extern volatile bool terminateProgram;

std::string ErrorMsg(int8_t errorNo, std::string msg);
void signalHandler(int signum);

void setupSignalHandling();

#endif