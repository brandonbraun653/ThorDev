/********************************************************************************
 *   File Name:
 *    main.cpp
 *
 *   Description:
 *    SPI development project entry
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/gpio.hpp>
#include <Chimera/spi.hpp>
#include <Chimera/system.hpp>
#include <Chimera/threading.hpp>
#include <Chimera/watchdog.hpp>

#include <Thor/drivers/f4/spi/hw_spi_driver.hpp>

void test_thread( void *arguments );
void spi_thread( void *arguments );

void startup_sequence( Chimera::GPIO::GPIOClass *const led );

int main( void )
{
  Chimera::System::initialize();

  Chimera::Threading::addThread( test_thread, "wd_thread", 1000, nullptr, 3, nullptr );
  Chimera::Threading::addThread( spi_thread, "spi_thread", 1000, nullptr, 3, nullptr );
  Chimera::Threading::startScheduler();
  return 0;
}

void test_thread( void *arguments )
{
  Chimera::Threading::signalSetupComplete();

  Chimera::GPIO::PinInit ledInit;
  ledInit.accessMode = Chimera::Hardware::AccessMode::THREADED;
  ledInit.drive      = Chimera::GPIO::Drive::OUTPUT_PUSH_PULL;
  ledInit.port       = Chimera::GPIO::Port::PORTA;
  ledInit.pull       = Chimera::GPIO::Pull::NO_PULL;
  ledInit.pin        = 5;

  auto led = Chimera::GPIO::GPIOClass();
  led.init( ledInit );
  led.setState( Chimera::GPIO::State::HIGH );

  startup_sequence( &led );

  auto watchdog = Chimera::Watchdog::WatchdogClass();
  watchdog.initialize( 500, 100 );
  watchdog.start();

  while ( 1 )
  {
    watchdog.kick();
    led.setState( Chimera::GPIO::State::HIGH );
    Chimera::delayMilliseconds( 150 );
    led.setState( Chimera::GPIO::State::LOW );
    Chimera::delayMilliseconds( 150 );
  }
}

void spi_thread( void *arguments )
{
  Chimera::Threading::signalSetupComplete();

  auto spi = Thor::Driver::SPI::Driver();


  while ( 1 )
  {
    Chimera::delayMilliseconds( 100 );
  }
}

void startup_sequence( Chimera::GPIO::GPIOClass *const led )
{
  for ( uint8_t i = 0; i < 5; i++ )
  {
    led->setState( Chimera::GPIO::State::HIGH );
    Chimera::delayMilliseconds( 65 );
    led->setState( Chimera::GPIO::State::LOW );
    Chimera::delayMilliseconds( 25 );
  }

  Chimera::delayMilliseconds( 350 );
}