/********************************************************************************
 *   File Name:
 *    main.cpp
 *
 *   Description:
 *    Watchdog development project entry
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/system.hpp>
#include <Chimera/threading.hpp>
#include <Chimera/watchdog.hpp>

void test_thread( void *arguments );

int main( void )
{
  Chimera::System::initialize();

  Chimera::Threading::addThread( test_thread, "wd_thread", 1000, nullptr, 3, nullptr );
  Chimera::Threading::startScheduler();
  return 0;
}

void test_thread( void *arguments )
{
  Chimera::Threading::signalSetupComplete();

  auto watchdog = Chimera::Watchdog::WatchdogClass();
  watchdog.initialize( 500, 100 );
  watchdog.start();

  while ( 1 )
  {
    watchdog.kick();
    Chimera::delayMilliseconds( 100 );
  }
}