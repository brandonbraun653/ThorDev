/******************************************************************************
 *  File Name:
 *    mbed_test_harness_cfg.hpp
 *
 *  Description:
 *    Test harness configuration file for Thor
 *
 *  2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef THOR_MBED_TEST_HARNESS_CONFIG_HPP
#define THOR_MBED_TEST_HARNESS_CONFIG_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#if __has_include( "mbed_test_harness_cfg_prj.hpp")
#include "mbed_test_harness_cfg_prj.hpp"
#endif

#include <Chimera/serial>

/*-----------------------------------------------------------------------------
Literals
-----------------------------------------------------------------------------*/
#ifndef THOR_TEST_SERIAL_BAUD
#define THOR_TEST_SERIAL_BAUD    ( 115200u )
#endif

#ifndef THOR_TEST_SERIAL_CHANNEL
#define THOR_TEST_SERIAL_CHANNEL ( Chimera::Serial::Channel::SERIAL6 )
#endif

#ifndef THOR_TEST_RUNNER_STACK_SIZE
#define THOR_TEST_RUNNER_STACK_SIZE ( 10 * 1024u )
#endif

#ifndef THOR_TEST_SERIAL_RX_BUFFER_SIZE
#define THOR_TEST_SERIAL_RX_BUFFER_SIZE ( 256u )
#endif

#ifndef THOR_TEST_SERIAL_TX_BUFFER_SIZE
#define THOR_TEST_SERIAL_TX_BUFFER_SIZE ( 1024u )
#endif

#endif /* !THOR_MBED_TEST_HARNESS_CONFIG_HPP */
