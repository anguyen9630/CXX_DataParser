# ScaleDataParser
C++ Serial Data Parser for Pacific Scales - MTData Trial

# Task Outline
A simulator written in python, it will output a block of strings to a serial port every two seconds.
1. Write a program in C++ which will parse this data. It should allow the user to specify a serial port to open on the command line.
2. The data should be converted into JSON format, with an extra boolean field called 'VALID' which will signify that the TOTAL field is correct.
3. On each 10 seconds system time boundary, eg 2:00:00pm, 2:00:10pm, 2:00:20pm the program should print the latest JSON data to stdout.
4. The program should run on Ubuntu and use a makefile to build.
5. You can use any libraries you wish to complete this task, please specify any packages which need to be installed to allow your program to build.
