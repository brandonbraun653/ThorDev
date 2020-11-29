/********************************************************************************
 *  File Name:
 *    main.cpp
 *
 *  Description:
 *    Sandbox entry for testing USB on the STM32L4 platform
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/thread>

/* Testing Includes */
#include "usb_driver.hpp"
#include <tests/hardware/harness/blinky.hpp>

/*-------------------------------------------------------------------------------
Public Functions
-------------------------------------------------------------------------------*/
int main()
{
  /*-------------------------------------------------
  Initialize system drivers
  -------------------------------------------------*/
  using namespace Chimera::Threading;
  ChimeraInit();

  /*-------------------------------------------------
  Initialize threads
  -------------------------------------------------*/
  using namespace Thor::Testing;
  Thread blinky;
  blinky.initialize( blinkyThread, nullptr, Priority::LEVEL_3, BLINKY_STACK, BLINKY_NAME );
  blinky.start();

  Thread sandbox;
  sandbox.initialize( USBTesting, nullptr, Priority::LEVEL_3, USBStack, USBName );
  sandbox.start();

  /*-------------------------------------------------
  Run the RTOS scheduler
  -------------------------------------------------*/
  startScheduler();
  return 0;
}
