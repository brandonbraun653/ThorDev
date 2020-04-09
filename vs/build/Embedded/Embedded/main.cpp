/********************************************************************************
 *   File Name:
 *       main.cpp
 *
 *   Description:
 *       Entry for hardware development and tests
 *
 *   2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <string>

/* Chimera Includes */
#include <Chimera/gpio>
//#include <Chimera/spi>
#include <Chimera/system>
#include <Chimera/thread>
//#include <Chimera/watchdog>
//
///* Thor Includes */
//#include <Thor/gpio>
//#include <Thor/lld/stm32f4x/gpio/hw_gpio_types.hpp>
//
///* Logger Includes */
//#include <uLog/ulog.hpp>
//#include <uLog/sinks/sink_cout.hpp>
//#include <uLog/sinks/sink_vgdb_semihosting.hpp>
//
///* Visual GDB Includes */
//#include "SysprogsProfiler.h"


using namespace Chimera::Threading;

static void background_thread( void *arg );
static void startup_blinky_sequence( const Chimera::GPIO::GPIO_sPtr &led );

int main( void )
{
  /*------------------------------------------------
  Boot up the device hardware
  ------------------------------------------------*/
  ChimeraInit();

  Chimera::System::Information *sysInfo = nullptr;
  Chimera::System::getSystemInformation( sysInfo );


  /*------------------------------------------------
  Initialize the profiler
  ------------------------------------------------*/
  //    InitializeInstrumentingProfiler();

  /*------------------------------------------------
  Initialize the logger system
  ------------------------------------------------*/
  //    uLog::initialize();
  //
  //    uLog::SinkHandle rootSink = std::make_shared<uLog::VGDBSemihostingSink>();
  //    rootSink->setLogLevel( uLog::Level::LVL_DEBUG );
  //    rootSink->setName( "Root" );
  //    rootSink->enable();
  //
  //    uLog::registerSink( rootSink );
  //    uLog::setGlobalLogLevel( uLog::Level::LVL_DEBUG );
  //    uLog::setRootSink( rootSink );
  //
  //    rootSink->flog( uLog::Level::LVL_INFO, "Boot up the world!\n" );

  /*------------------------------------------------
  Create the system threads
  ------------------------------------------------*/
  Thread blinkyThread;
  blinkyThread.initialize( background_thread, nullptr, Priority::LEVEL_3, 500, "blinky" );
  blinkyThread.start();

  startScheduler();
}

void startup_blinky_sequence( const Chimera::GPIO::GPIO_sPtr &led )
{
  for ( uint8_t i = 0; i < 5; i++ )
  {
    led->setState( Chimera::GPIO::State::HIGH, 100 );
    Chimera::delayMilliseconds( 65 );
    led->setState( Chimera::GPIO::State::LOW, 100 );
    Chimera::delayMilliseconds( 25 );
  }

  Chimera::delayMilliseconds( 350 );
}

void background_thread( void *arguments )
{
  /*------------------------------------------------
  Initialize the LED gpio
  ------------------------------------------------*/
  Chimera::GPIO::PinInit ledInit;
  ledInit.threaded = false;
  ledInit.validity = true;
  ledInit.drive    = Chimera::GPIO::Drive::OUTPUT_PUSH_PULL;
  ledInit.port     = Chimera::GPIO::Port::PORTB;
  ledInit.pull     = Chimera::GPIO::Pull::NO_PULL;
  ledInit.pin      = 3;

  auto led = Chimera::GPIO::create_shared_ptr();
  led->init( ledInit, TIMEOUT_DONT_WAIT );
  led->setState( Chimera::GPIO::State::HIGH, TIMEOUT_DONT_WAIT );

  startup_blinky_sequence( led );

  while ( 1 )
  {
    led->setState( Chimera::GPIO::State::HIGH, 100 );
    Chimera::delayMilliseconds( 150 );
    led->setState( Chimera::GPIO::State::LOW, 100 );
    Chimera::delayMilliseconds( 150 );
  }
}