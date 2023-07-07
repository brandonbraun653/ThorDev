/******************************************************************************
 *  File Name:
 *    cpputest_config_prj.hpp
 *
 *  Description:
 *    Project configuration for CppUTest
 *
 *  2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef THOR_DEV_CPPUTEST_CONFIG_HPP
#define THOR_DEV_CPPUTEST_CONFIG_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <tests/harness/mbed_test_harness_cfg.hpp>

/*-----------------------------------------------------------------------------
Constants
-----------------------------------------------------------------------------*/
#define CPPUTEST_PLATFORM_SERIAL_CHANNEL  ( THOR_TEST_SERIAL_CHANNEL )

#endif  /* !THOR_DEV_CPPUTEST_CONFIG_HPP */
