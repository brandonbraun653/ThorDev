/********************************************************************************
 *  File Name:
 *    test_driver.cpp
 *
 *  Description:
 *    Primary test driver for Thor HLD CAN
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/


/* Testing Includes */
#include <gtest/gtest.h>

namespace Thor::HLD::CAN
{
  TEST(HelloWorld, DoesItWork)
  {
    EXPECT_EQ( 2, 2 );
  }
}  // namespace Thor::HLD::CAN
