/********************************************************************************
 *  File Name:
 *    echo_test.hpp
 *
 *  Description:
 *    CAN echo test interface
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef THOR_HLD_CAN_TESTING_ECHO_HPP
#define THOR_HLD_CAN_TESTING_ECHO_HPP

/* Chimera Includes */
#include <Chimera/can>

namespace Thor::CAN::Testing
{
  void startEchoTest();

  void stopEchoTest();

}  // namespace Thor::CAN::Testing

#endif  /* !THOR_HLD_CAN_TESTING_ECHO_HPP */
