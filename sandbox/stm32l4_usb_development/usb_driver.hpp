/********************************************************************************
 *  File Name:
 *    usb_driver.hpp
 *
 *  Description:
 *    Test thread for USB
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef THOR_TEST_THREAD_USB_HPP
#define THOR_TEST_THREAD_USB_HPP

/* STL Includes */
#include <cstddef>
#include <string_view>

/* Chimera Includes */
#include <Chimera/thread>

/*-------------------------------------------------------------------------------
Constants
-------------------------------------------------------------------------------*/
static constexpr size_t USBStack          = STACK_KILOBYTES( 10 );
static constexpr std::string_view USBName = "UsbTest";

/*-------------------------------------------------------------------------------
Public Functions
-------------------------------------------------------------------------------*/
void USBTesting( void *arg );

#endif /* !THOR_TEST_THREAD_USB_HPP */
