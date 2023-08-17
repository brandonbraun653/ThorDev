/********************************************************************************
 *  File Name:
 *    blinky.cpp
 *
 *  Description:
 *    Test harness blinky thread implementation
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/gpio>

/* Testing Includes */
#include <tests/hardware/harness/blinky.hpp>

namespace Thor::Testing
{
  /*-------------------------------------------------------------------------------
  Constants
  -------------------------------------------------------------------------------*/
  static constexpr Chimera::GPIO::Pin pin   = 14;
  static constexpr Chimera::GPIO::Port port = Chimera::GPIO::Port::PORTC;

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  void blinkyThread( void *arg )
  {
    using namespace Chimera::GPIO;

    /*-------------------------------------------------
    Configure the GPIO
    -------------------------------------------------*/
    PinInit cfg;
    cfg.clear();
    cfg.validity  = true;
    cfg.threaded  = true;
    cfg.alternate = Alternate::NONE;
    cfg.drive     = Drive::OUTPUT_PUSH_PULL;
    cfg.pin       = pin;
    cfg.port      = port;
    cfg.pull      = Pull::NO_PULL;
    cfg.state     = State::LOW;

    auto pin = getDriver( cfg.port, cfg.pin );
    pin->init( cfg );
    pin->setState( State::LOW );

    /*-------------------------------------------------
    Run the heartbeat animation
    -------------------------------------------------*/
    while ( 1 )
    {
      /*-------------------------------------------------
      High pulse #1
      -------------------------------------------------*/
      pin->setState( State::HIGH );
      Chimera::delayMilliseconds( 100 );
      pin->setState( State::LOW );
      Chimera::delayMilliseconds( 100 );

      /*-------------------------------------------------
      High pulse #2
      -------------------------------------------------*/
      pin->setState( State::HIGH );
      Chimera::delayMilliseconds( 100 );
      pin->setState( State::LOW );
      Chimera::delayMilliseconds( 100 );

      /*-------------------------------------------------
      Hold longer time in off state
      -------------------------------------------------*/
      Chimera::delayMilliseconds( 450 );
    }
  }
}    // namespace Thor::Testing
