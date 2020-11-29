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
  cfg.clear();
  cfg.channel       = Channel::USB0;
  cfg.useLPM        = false;
  cfg.useBattCharge = false;

  /*-------------------------------------------------
  GPIO Configuration
  -------------------------------------------------*/
  cfg.pinDP.alternate = Chimera::GPIO::Alternate::USB_DP;
  cfg.pinDP.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  cfg.pinDP.pin       = 12;
  cfg.pinDP.port      = Chimera::GPIO::Port::PORTA;
  cfg.pinDP.pull      = Chimera::GPIO::Pull::NO_PULL;
  cfg.pinDP.threaded  = true;
  cfg.pinDP.validity  = true;

  cfg.pinDM.alternate = Chimera::GPIO::Alternate::USB_DM;
  cfg.pinDM.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  cfg.pinDM.pin       = 11;
  cfg.pinDM.port      = Chimera::GPIO::Port::PORTA;
  cfg.pinDM.pull      = Chimera::GPIO::Pull::NO_PULL;
  cfg.pinDM.threaded  = true;
  cfg.pinDM.validity  = true;


  volatile auto result  = Chimera::USB::init( cfg.channel, cfg );
  volatile auto result1 = Chimera::USB::destroy( cfg.channel );

  while ( 1 )
  {
    Chimera::Threading::this_thread::yield();
  }
}