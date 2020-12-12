/********************************************************************************
 *  File Name:
 *    test_hld_adc_hw.cpp
 *
 *  Description:
 *    Sandbox for testing the HLD interface to ADC hardware
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/adc>
#include <Chimera/gpio>
#include <Chimera/serial>
#include <Chimera/thread>

/* Thor Includes */
#include <Thor/adc>

/* Testing Includes */
#include <tests/hardware/harness/blinky.hpp>
#include <tests/hardware/harness/logger.hpp>

/*-------------------------------------------------------------------------------
Static Functions & Data
-------------------------------------------------------------------------------*/
static void test_thread( void *argument );


/*-------------------------------------------------------------------------------
Public Functions
-------------------------------------------------------------------------------*/
int main()
{
  using namespace Chimera::Threading;
  ChimeraInit();

  /*-------------------------------------------------
  Create the blinky thread
  -------------------------------------------------*/
  Thread blinky;
  blinky.initialize( Thor::Testing::blinkyThread, nullptr, Priority::LEVEL_1, Thor::Testing::BLINKY_STACK,
                     Thor::Testing::BLINKY_NAME.cbegin() );
  blinky.start();

  /*-------------------------------------------------
  Create the test thread
  -------------------------------------------------*/
  Thread testing;
  testing.initialize( test_thread, nullptr, Priority::LEVEL_3, STACK_KILOBYTES( 10 ), "test" );
  testing.start();

  startScheduler();
  return 0;
}


static void test_thread( void *argument )
{
  /*-------------------------------------------------
  Initialize the Logger
  -------------------------------------------------*/
  Thor::Testing::initializeLogger( uLog::Level::LVL_DEBUG );

  /*-------------------------------------------------------------------------------
  Initialize the ADC driver
  -------------------------------------------------------------------------------*/


  /*-------------------------------------------------------------------------------
  Idle away into nothing
  -------------------------------------------------------------------------------*/
  while ( 1 )
  {
    Chimera::delayMilliseconds( 100 );
  }
}