/********************************************************************************
 *  File Name:
 *    usb_driver.cpp
 *
 *  Description:
 *    USB development implementation details
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/thread>
#include <Chimera/usb>


/* Testing Includes */
#include "usb_driver.hpp"


void USBTesting( void *arg )
{
  using namespace Chimera::USB;

  PeriphConfig cfg;
  cfg.channel = Channel::USB0;


  Chimera::USB::init( cfg.channel, cfg );

  while ( 1 )
  {
    Chimera::Threading::this_thread::yield();
  }
}