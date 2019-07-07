/********************************************************************************
 *   File Name:
 *       main.cpp
 *
 *   Description:
 *       Entry point for embedded development of the custom Serial (USART/UART)
 *       driver for the STM32F4 series of devices.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/gpio.hpp>
#include <Chimera/threading.hpp>

//#include <Thor/drivers/GPIO.hpp>
#include <Thor/drivers/RCC.hpp>

using namespace Chimera::GPIO;
using namespace Chimera::Threading;

void testThread( void *argument );

int main()
{
  Thor::Driver::RCC::init();
  auto sys = Thor::Driver::RCC::SystemClock::get();
  sys->setCoreClockSource( Thor::Clock::Source::HSE );
  sys->setCoreClock( 160000000 );

  volatile auto tmp = sys->getCoreClock();

  addThread( testThread, "testThread", 500, nullptr, 2, nullptr );

  startScheduler();

  while ( 1 ) {}
  return 0;
}

void testThread( void *argument )
{
  GPIOClass gpio;
  PinInit init;

  init.drive      = Drive::OUTPUT_PUSH_PULL;
  init.pin        = 5;
  init.port       = Port::PORTA;
  init.pull       = Pull::NO_PULL;
  init.state      = State::LOW;
  init.accessMode = Chimera::Hardware::AccessMode::THREADED;

  gpio.init( init );

  signalSetupComplete();

  while ( 1 )
  {
    gpio.setState( State::HIGH );
    Chimera::delayMilliseconds( 500 );
    
    gpio.setState( State::LOW );
    Chimera::delayMilliseconds( 500 );
  }
}