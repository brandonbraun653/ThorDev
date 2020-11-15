/********************************************************************************
 *  File Name:
 *    blinky.hpp
 *
 *  Description:
 *    Blinky thread to let tester know the hardware hasn't died
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef THOR_TESTING_HARNESS_BLINKY_THREAD_HPP
#define THOR_TESTING_HARNESS_BLINKY_THREAD_HPP

/* STL Includes */
#include <cstddef>
#include <string_view>

/* Chimera Includes */
#include <Chimera/thread>

namespace Thor::Testing
{
  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
  static constexpr size_t BLINKY_STACK          = STACK_BYTES( 250 );
  static constexpr std::string_view BLINKY_NAME = "blinky";

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  void blinkyThread( void *arg );

}    // namespace Thor::Testing

#endif /* !THOR_TESTING_HARNESS_BLINKY_THREAD_HPP */
