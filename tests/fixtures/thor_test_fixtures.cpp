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
#include "mock_stm32_hal_gpio.hpp"

namespace Chimera
{
  namespace IntegrationTests
  {
    void CommonSetUp()
    {

    }

    void CommonTearDown()
    {
      STM32HAL_Mock::gpioMockObj.reset();
    }

  }
}