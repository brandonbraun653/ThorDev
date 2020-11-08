/********************************************************************************
 *  File Name:
 *    serial_output.hpp
 *
 *  Description:
 *    Provides an interface for configuring a serial channel commonly used on
 *    a lot of the system hardware tests.
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef THOR_TESTING_SERIAL_CONFIG_HPP
#define THOR_TESTING_SERIAL_CONFIG_HPP

/* Chimera Includes */
#include <Chimera/serial>


namespace Thor::Testing
{
  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
  static constexpr Chimera::Serial::Channel serialChannel = Chimera::Serial::Channel::SERIAL1;

  /*-------------------------------------------------------------------------------
  Public Methods
  -------------------------------------------------------------------------------*/
  /**
   *  Initializes the serial output on the serialChannel
   *  @return void
   */
  void initializeSerial();

}    // namespace Thor::Testing

#endif /* !THOR_TESTING_SERIAL_CONFIG_HPP */
