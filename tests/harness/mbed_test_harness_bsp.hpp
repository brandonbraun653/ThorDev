/******************************************************************************
 *  File Name:
 *    mbed_test_harness_bsp.hpp
 *
 *  Description:
 *    Board support package for the test harness
 *
 *  2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

#pragma once
#ifndef THOR_MBED_TEST_HARNESS_BSP_HPP
#define THOR_MBED_TEST_HARNESS_BSP_HPP

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#if __has_include( "mbed_test_harness_bsp_prj.hpp")
#include "mbed_test_harness_bsp_prj.hpp"
#else 
// TODO: Need to use the preprocessor to redirect to the board target
#warning "Missing project specific BSP file"
#include <tests/config/bsp/orbit_esc_v3.hpp>
#endif 

#endif  /* !THOR_MBED_TEST_HARNESS_BSP_HPP */
