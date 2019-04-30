/********************************************************************************
 *  File Name:
 *    thor_test_fixtures.cpp
 *
 *  Description:
 *    Defines common resources used in testing Thor
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Thor Includes */
#include "thor_test_fixtures.hpp"

/* Chimera Includes */
#include <Chimera/test/fixtures/common_fixture.hpp>

/* STM32 Mock Includes */
#include "mock_stm32_hal_cortex.hpp"
#include "mock_stm32_hal_gpio.hpp"
#include "mock_stm32_hal_spi.hpp"
#include "mock_stm32_hal_uart.hpp"
#include "mock_stm32_hal_usart.hpp"

namespace Chimera
{
  namespace IntegrationTests
  {
    void CommonSetUp()
    {
      using namespace STM32HAL_Mock;

      cortexMockObj = std::make_shared<CortexNiceMock>();
    }

    void CommonTearDown()
    {
      using namespace STM32HAL_Mock;

      cortexMockObj.reset();
      gpioMockObj.reset();
      spiMockObj.reset();
      uartMockObj.reset();
      usartMockObj.reset();
    }

  }
}