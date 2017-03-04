/*
 * TestFrame.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Lars Schwensen
 */

//STEP 1: Browse the project folder to:
//googleTest -> googletest -> scripts

//STEP 2: run in command line:
// ./fuse_gtest_files.py ../../../googleTestLib

//STEP 3: refresh the Project (F5)

//STEP 4: Rightclick on folder 'googleTest' Resouce Configurations -> Exclude from Build... and select Debug and Release
//STEP 4.5: Rightclick on folder 'googleTestLib' Resouce Configurations -> Exclude from Build... and select Release

//STEP 5: Include pthread
// Rightclick on project -> Properties -> C/C++ Build -> Settings -> ToolSettings -> GCC C++ Compiler ->
// Miscellaneous and add '-pthread' to other flags.

//STEP 5.5: Include pthread
// Rightclick on project -> Properties -> C/C++ Build -> Settings -> ToolSettings -> GCC C++ Linker ->
// Libraries and add 'pthread' to other libraries (-l).

#include "../googleTestLib/gtest/gtest.h"

//INCLUDE YOUR TESTCASES HERE!
//#include "MessageTest.h"
//#include "ProcessTest.h"
//#include "NetworkModulTest.h"
#include "CommandExecutorTestCase.h"

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
