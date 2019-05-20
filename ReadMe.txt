******************************************************************************************************************
Author:	Al Timofeyev
Date:	April 19, 2019
Desc:	This is how to compile and execute the code.
******************************************************************************************************************

**********************************************************************************
NOTES TO PROFESSOR (if any) ARE AT THE VERY BOTTOM OF THIS README!!
**********************************************************************************


**********************************************************************************
---------------------------- ENVIRONMENT USED TO CODE ----------------------------
Windows 10
CLion version 2019.1
cygwin version 3.0.4
cygwin GDB version 8.1.1
gcc version 7.4.0 
g++ version 7.4.0

******
NOTE:
1)	CLion generated a CMakeLists.txt file included with the source code.
	cmake_minimum_required(VERSION 3.13)
2)	The program was written in C++.
******
**********************************************************************************
**********************************************************************************
--------------------------- SETUP CONFIGURATION FILES ----------------------------
---- Structure of configuration file
<list of dimensions>				---- First line only
<list of function IDs and their bounds>		---- All other lines.
Use only a comma (,) delimiter, no spaces between values.

-- Example:
10,20,30		---- First line is a list of 10, 20, and 30 dimiensions.
1,-500,40		---- Second line is for Benchmark Function 1, with -500/40 min/max bounds.
5,-32,100		---- Third line is for Benchmark Function 5, with -32/100 min/max bounds.
8,0,pi,0		---- Fourth line is Benchmark Function 8, with 0/pi min/max bounds.
			     \--> PLEASE LOOK AT NOTE 3 IN THIS SECTION.


---- Structure of configuratoin file for Search Algorithms
<number of iterations>		---- First line is for number of iteration.
<alpha value>			---- Second line is for a alpha value for neighborhood mutation.

-- Example:
30			---- First line is set to 30 iteration.
0.11			---- Second line sets the alpha = 0.11.

******
NOTE:
1)	Depending on which IDE you are running, conig files should be either
	in the same folder as source code or in build folder.
2)	Configuration files can be passed as command line parameters or use the default
	configuration file (just alter it).
3)	Please note the extra zero (0) value after pi (in above example). On any line
	that contains the value pi, please include an extra value, like zero. This is
	for conversion purposes, otherwise the program will not run.
******
**********************************************************************************
**********************************************************************************
------------------------------ COMPILE AND EXECUTE -------------------------------
---- To compile for an IDE project.
To Compile:
You could use CMake to compile CMakeLists.txt file that's included with source code.

To Execute:
run main.cpp


---- I'm assuming it could also be compiled and run from command line:
To Compile:
g++ -o main main.cpp

To Execute:
./main				---- Default config.txt is used as configuration file.
./main confrog.txt		---- confrog.txt is configuration file example.
./main configFile2.txt		---- configFile2.txt is configuration file example.
./main blabla.txt		---- blabla.txt is configuration file example.

******
NOTE:
1) main.cpp is just a test driver for the ProcessFunctions class.
   Feel free to change and add to it to generate resutls.
2) Every time main.cpp is executed, the previous files that held
   results will be overwritten if the same configuration file is used.
******
**********************************************************************************



**********************************************************************************
------------------------------ NOTES TO PROFESSOR --------------------------------
1)	You can have only ONE configuration file for Search Algorithms BUT multiple
	configuration files, which contain the number of dimensions etc., with which
	to start the program.

2)	The configuration file used to start the program can be passed as a command
	line parameter or you can change the default config.txt file.

3)	If you wish to see all possible function IDs and their respective function
	names printed to the screen, uncomment line 47 of main.cpp. This can be
	helpful when making the configuration file(s) for this program.

4)	For the sake of conserving storage space on your computer, I have commented
	out line 69 of main.cpp. This way, only the analyzed results will be saved
	to file. If you wish to save to file All the fitness' and matrices of each
	dimension, uncomment line 69 of main.cpp.

5)	The files are saved based on the name of the configuration file that was
	used to start the program, and the number of dimensions the file was
	generated for.
**********************************************************************************