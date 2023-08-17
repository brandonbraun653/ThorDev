/******************************************************************************
 *  File Name:
 *    mbed_test_harness_intf.hpp
 *
 *  Description:
 *    Interface functions required by the test harness and implemented by the
 *    test driver.
 *
 *  2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef THOR_MBED_TEST_HARNESS_INTF_HPP
#define THOR_MBED_TEST_HARNESS_INTF_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/common>

namespace Thor::Testing::Driver
{
  /*---------------------------------------------------------------------------
  Public Functions
  ---------------------------------------------------------------------------*/
  /**
   * @brief Initializes the test driver with test specific configuration
   * @return Chimera::Status_t 
   */
  Chimera::Status_t setUp();

  /**
   * @brief Destroys the test driver and cleans up any resources
   * @return Chimera::Status_t 
   */
  Chimera::Status_t tearDown();

  /**
   * @brief Returns the name of the test being run
   * @return const char* 
   */
  const char *getTestName();

}    // namespace Thor::Testing::Driver

#endif /* !THOR_MBED_TEST_HARNESS_INTF_HPP */
