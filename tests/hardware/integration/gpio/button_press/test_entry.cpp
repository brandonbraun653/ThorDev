/********************************************************************************
 *  File Name:
 *    test_entry.cpp
 *
 *  Description:
 *    Tests the HLD GPIO driver on the target device
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <array>
#include <cstring>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/gpio>
#include <Chimera/serial>
#include <Chimera/thread>

/* Testing Includes */
#include <tests/hardware/harness/logger.hpp>

/* CppUTest Includes */
#include <CppUTest/CommandLineTestRunner.h>

/*-------------------------------------------------------------------------------
Static Declarations
-------------------------------------------------------------------------------*/
static void test_thread( void *argument );


/*-------------------------------------------------------------------------------
Public Functions
-------------------------------------------------------------------------------*/
int main()
{
  using namespace Chimera::Threading;
  ChimeraInit();

  Thread testing;
  testing.initialize( test_thread, nullptr, Priority::LEVEL_3, STACK_KILOBYTES( 10 ), "test" );
  testing.start();

  startScheduler();
  return 0;
}


static void test_thread( void *argument )
{
  /*-------------------------------------------------
  Configure the output style:
    - Colorized
    - Verbose
  -------------------------------------------------*/
  const char *av_override[] = { "-c", "-v" };

  /*-------------------------------------------------
  Start up the logger
  -------------------------------------------------*/
  Thor::Testing::initializeLogger( uLog::Level::LVL_DEBUG );
  auto logger = uLog::getRootSink();

  /*-------------------------------------------------
  Execute the tests
  -------------------------------------------------*/
  logger->flog( uLog::Level::LVL_INFO, "\rStarting STM32 HLD GPIO Tests\r\n" );
  int rcode = CommandLineTestRunner::RunAllTests( 2, av_override );
  logger->flog( uLog::Level::LVL_INFO, "Test exit with code: %d\r\n", rcode );

  /*-------------------------------------------------
  Idle away to infinity
  -------------------------------------------------*/
  while ( 1 )
  {
    Chimera::insert_debug_breakpoint();
    Chimera::Threading::this_thread::yield();
  }
}
