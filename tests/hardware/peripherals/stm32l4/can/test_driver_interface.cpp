/********************************************************************************
 *  File Name:
 *    test_driver_interface.cpp
 *
 *  Description:
 *    Tests the STM32L4 LLD CAN Interface
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Thor Includes */
#include <Thor/lld/stm32l4x/can/hw_can_prj.hpp>
#include <Thor/lld/stm32l4x/can/hw_can_prv_driver.hpp>
#include <Thor/lld/stm32l4x/can/hw_can_types.hpp>

/* CppUTest Includes */
#include <CppUTest/TestHarness.h>

/*-------------------------------------------------------------------------------
Testing Functions
-------------------------------------------------------------------------------*/
TEST_GROUP( STM32L4_LLD_CAN ){};

TEST( STM32L4_LLD_CAN, Open )
{
  CHECK_EQUAL( 1, 1 );
}
