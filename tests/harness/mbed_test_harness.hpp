/******************************************************************************
 *  File Name:
 *    embedded_test_harness.hpp
 *
 *  Description:
 *    Test harness interface for running CppUTest drivers on-target
 *
 *  2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef THOR_EMBEDDED_TEST_HARNESS_HPP
#define THOR_EMBEDDED_TEST_HARNESS_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <tests/harness/mbed_test_harness_cfg.hpp>


namespace Thor::Testing
{
  /**
   * @brief Main entry point for the testing engine
   * @return int
   */
  int ExecuteMainTestThread();
}  // namespace Thor::Testing

#endif  /* !THOR_EMBEDDED_TEST_HARNESS_HPP */
