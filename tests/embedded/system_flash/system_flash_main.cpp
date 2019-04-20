/********************************************************************************
 *  File Name:
 *    system_flash_main.cpp
 *
 *  Description:
 *    Testing entry point for the Thor internal system flash driver 
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Testing Includes */
#include "TinyEmbeddedTest.h"

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/threading.hpp>

/* Profiler Includes */
#include "SysprogsProfiler.h"

static void testThread( void *arguments );

int main(void)
{
  ChimeraInit();

  InitializeSamplingProfiler();
  InitializeInstrumentingProfiler();

  Chimera::Threading::addThread( testThread, "tt", 1000, NULL, 2, NULL );
  Chimera::Threading::startScheduler();
  
  /* We should never hit this. The real exit is in the test thread. */
  return 0;
}

static void testThread( void *arguments )
{
  Chimera::Threading::signalSetupComplete();

  RunAllTests();

  exit( 0 );
}