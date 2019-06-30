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

#include <Thor/drivers/GPIO.hpp>
#include <Thor/drivers/RCC.hpp>

using namespace Chimera::GPIO;
using namespace Thor::Driver::GPIO;

static constexpr uint8_t pin = 5;
int main()
{
  Thor::Driver::RCC::init();


  // Do an led thing
  uint32_t counter = 0;

  DriverBare gpio;

  gpio.attach( GPIOA_PERIPH );
  gpio.driveSet( pin, Chimera::GPIO::Drive::OUTPUT_PUSH_PULL );
  gpio.pullSet( pin, Chimera::GPIO::Pull::NO_PULL );
  gpio.speedSet( pin, Thor::Driver::GPIO::Speed::FAST );
  gpio.alternateFunctionSet( pin, AF_NONE );


  while ( 1 )
  {
    gpio.write( pin, State::HIGH );
    gpio.write( pin, State::LOW );

    counter++;
    counter--;
  }
  return 0;
}