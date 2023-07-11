# ScaleDataParser
C++ Serial Data Parser for Pacific Scales - MTData Trial

# Task Outline
A simulator written in python, it will output a block of strings to a serial port every two seconds.
1. Write a program in C++ which will parse this data. It should allow the user to specify a serial port to open on the command line.
2. The data should be converted into JSON format, with an extra boolean field called 'VALID' which will signify that the TOTAL field is correct.
3. On each 10 seconds system time boundary, eg 2:00:00pm, 2:00:10pm, 2:00:20pm the program should print the latest JSON data to stdout.
4. The program should run on Ubuntu and use a makefile to build.
5. You can use any libraries you wish to complete this task, please specify any packages which need to be installed to allow your program to build.

# Requirements
Software required to run this program
## Simulator
- Python3
- Python-pip3
- pyserial
- dataparser
- git
## Data parser
- C++17
- make
- cmake (for json)
- [JSON by Niels Lohmann](https://github.com/nlohmann/json)
- git
- build-essential
# Build
To build the program you must first build the json library. To start clone the library with
```
git clone https://github.com/nlohmann/json.git
```
Then navigate into the repo folder
```
cd json
```
Create a build folder, run cmake, build and install
```
mkdir build && cd build && cmake .. && make && sudo make install
```
Return to the top directory and clone this git
```
cd ../.. && git clone https://github.com/anguyen9630/ScaleDataParser.git
```
Get into the repo and build
```
cd ScaleDataParser && make
```
# Usage 
```
scaleparser [-h|--help]
            [-p|--port <path>][-b|--baud <number>]
            [-i|--interval <time(s) [default: 10]>]
```
> -h|--help : Print help on the screen.
> -p|--port : Required. Defines the path to the serial port.
> -b|--baud : Required. Defines the baud rate.
> -i|--interval : Optional, defines the interval in which the program print the json data. Default at 10.

Reminder to set read/write permission to the serial port before using this program. This can be done with:
```
sudo chmod 777 <path_to_serial>
```
