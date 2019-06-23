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

using namespace Chimera::GPIO;
using namespace Thor::Driver::GPIO;
int main()
{
  // Do an led thing
  uint32_t counter = 0;

  DriverBare gpio;

  gpio.attach( GPIOA_PERIPH );

  while (1) 
  {
    counter++;
    counter--;
  }
  return 0;
}