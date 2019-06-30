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

//#include <Thor/drivers/GPIO.hpp>
#include <Thor/drivers/RCC.hpp>

using namespace Chimera::GPIO;

int main()
{
  Thor::Driver::RCC::init();


  // Do an led thing
  uint32_t counter = 0;

  GPIOClass gpio;
  PinInit init;

  init.drive = Drive::OUTPUT_PUSH_PULL;
  init.pin   = 5;
  init.port  = Port::PORTA;
  init.pull  = Pull::NO_PULL;
  init.state = State::LOW;

  gpio.init( init );

  while ( 1 )
  {
    gpio.setState( State::HIGH );
    gpio.setState( State::LOW );

    counter++;
    counter--;
  }
  return 0;
}