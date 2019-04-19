/********************************************************************************
 *  File Name:
 *    GTest_SystemFlash_F446RE.cpp
 *
 *  Description:
 *    Testing entry point for the system flash driver 
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Testing Includes */
#include <gtest/gtest.h>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/power.hpp>

/* Profiler Includes */
#include "SysprogsProfiler.h"

int main(void)
{
  ChimeraInit();

  InitializeSamplingProfiler();
  InitializeInstrumentingProfiler();

  char *argv = (char *)"";
	int argc = 1;
	testing::InitGoogleTest(&argc, &argv);
	(void)RUN_ALL_TESTS();
	return 0;
}
