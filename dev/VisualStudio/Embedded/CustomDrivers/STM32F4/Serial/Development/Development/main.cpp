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
#include <Chimera/system.hpp>
#include <Chimera/gpio.hpp>
#include <Chimera/threading.hpp>
#include <Chimera/serial.hpp>

using namespace Chimera::GPIO;
using namespace Chimera::Serial;
using namespace Chimera::Threading;

void blinkyThread( void *argument );
void serialThread( void *argument );

int main()
{
  Chimera::System::initialize();

  addThread( blinkyThread, "blinky", 500, nullptr, 2, nullptr );
  addThread( serialThread, "serial", 500, nullptr, 2, nullptr );

  startScheduler();

  while ( 1 ) {}
  return 0;
}

void blinkyThread( void *argument )
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

#include <Thor/drivers/f4/usart/hw_usart_driver.hpp>
#include <Thor/drivers/common/types/serial_types.hpp>
#include <Thor/drivers/f4/rcc/hw_rcc_driver.hpp>

void serialThread( void *argument )
{
  using namespace Thor::Driver::USART;
  using namespace Thor::Driver::Serial;
  using namespace Thor::Driver::RCC;

  Config cfg;
  cfg.BaudRate = 115200;


  Driver usart( USART3_PERIPH, USARTPeriph::get() );

  usart.init( cfg );

  while ( 1 )
  {
    

    Chimera::delayMilliseconds( 100 );
  }
}