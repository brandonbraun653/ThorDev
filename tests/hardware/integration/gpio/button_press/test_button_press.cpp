/********************************************************************************
 *  File Name:
 *    test_button_press.cpp
 *
 *  Description:
 *    Tests the STM32 HLD GPIO external interrupt capabilities
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Aurora Includes */
#include <Aurora/logging>
#include <Aurora/hmi>

/* Chimera Includes */
#include <Chimera/gpio>
#include <Chimera/exti>
#include <Chimera/function>
#include <Chimera/scheduler>

/* CppUTest Includes */
#include <CppUTest/TestHarness.h>

/*-------------------------------------------------------------------------------
Static Functions & Data
-------------------------------------------------------------------------------*/
/**
 *  This test suite was configured using a Luminary v2 board, which has
 *  Schmitt trigger inputs and helps with debouncing the signal a bit.
 *  In addition to this, the triggers are inverting and their output will
 *  default to HI because the inputs are pulled to GND via 50k resistors.
 *
 *  Note that the GPIO inputs to Luminary have a 50k resistor in series
 *  with the Schmitt trigger output, so it doesn't have much drive strength.
 */
static const Chimera::GPIO::PinInit pinCfg = { .alternate = Chimera::GPIO::Alternate::NONE,
                                               .drive     = Chimera::GPIO::Drive::INPUT,
                                               .pin       = 1,
                                               .port      = Chimera::GPIO::Port::PORTA,
                                               .pull      = Chimera::GPIO::Pull::NO_PULL,
                                               .state     = Chimera::GPIO::State::HIGH,
                                               .threaded  = true,
                                               .validity  = true };


TEST_GROUP( STM32_HLD_GPIO_EXTI ){};

/*-------------------------------------------------------------------------------
Debounce-less GPIO
-------------------------------------------------------------------------------*/
namespace Debounceless
{
  static volatile bool sISRFired     = false;
  static volatile size_t sPressCount = 0;

  static void debounceless( void *arg )
  {
    sISRFired = true;
    sPressCount++;
  }
  static auto cb_NoDebounce = etl::delegate<void( void * )>::create<debounceless>();


  TEST( STM32_HLD_GPIO_EXTI, UserButtonPress )
  {
    /*-------------------------------------------------
    Reset the test
    -------------------------------------------------*/
    sISRFired = false;

    /*-------------------------------------------------
    Configure the GPIO input pin
    -------------------------------------------------*/
    auto driver = Chimera::GPIO::getDriver( pinCfg.port, pinCfg.pin );
    driver->init( pinCfg );
    driver->attachInterrupt( cb_NoDebounce, Chimera::EXTI::EdgeTrigger::RISING_EDGE );

    /*-------------------------------------------------
    Execute the test
    -------------------------------------------------*/
    uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "\r\nTesting GPIO edge triggering functionality\r\n" );
    uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "Please press and hold the button\r\n" );

    while ( !sISRFired )
    {
      Chimera::delayMilliseconds( 100 );
      continue;
    }

    uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "Detected button press!\r\n" );
    CHECK( sISRFired );
    driver->detachInterrupt();
  }


  TEST( STM32_HLD_GPIO_EXTI, SpecificPressAmount )
  {
    constexpr size_t PressCount = 10;

    /*-------------------------------------------------
    Initialize the ISR data
    -------------------------------------------------*/
    sISRFired   = false;
    sPressCount = 0;

    /*-------------------------------------------------
    Configure the GPIO input pin
    -------------------------------------------------*/
    auto driver = Chimera::GPIO::getDriver( pinCfg.port, pinCfg.pin );
    driver->init( pinCfg );
    driver->attachInterrupt( cb_NoDebounce, Chimera::EXTI::EdgeTrigger::RISING_EDGE );

    /*-------------------------------------------------
    Execute the test
    -------------------------------------------------*/
    uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "Testing non-debounced GPIO edge triggering functionality\r\n" );
    uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "Please press the button %d times\r\n", PressCount );

    size_t lastCount = 0;
    while ( sPressCount < PressCount )
    {
      if ( lastCount != sPressCount )
      {
        lastCount = sPressCount;
        uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "%d\r\n", sPressCount );
      }

      Chimera::delayMilliseconds( 100 );
      continue;
    }

    uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "Thank you!\r\n" );
    CHECK( sPressCount == PressCount );
    driver->detachInterrupt();
  }

}    // namespace Debounceless


/*-------------------------------------------------------------------------------
Debounced GPIO
-------------------------------------------------------------------------------*/
namespace Debounced
{
  /*-------------------------------------------------
  Constants
  -------------------------------------------------*/
  static constexpr size_t DebounceTime = 150;
  static constexpr size_t SampleRate   = 10;
  static constexpr size_t NumSamples   = DebounceTime / SampleRate;

  /*-------------------------------------------------
  Variables
  -------------------------------------------------*/
  static volatile size_t NumPressed = 0;

  static void usercallback( Aurora::HMI::Button::ActiveEdge edge )
  {
    NumPressed++;
  }
  static auto cb_user = Aurora::HMI::Button::EdgeCallback::create<usercallback>();


  TEST( STM32_HLD_GPIO_EXTI, DebouncedGPIO )
  {
    using namespace Aurora::HMI;
    constexpr size_t PressCount = 10;

    /*-------------------------------------------------
    Initialize the ISR data
    -------------------------------------------------*/
    NumPressed = 0;

    /*-------------------------------------------------
    Initialize the debouncing driver
    -------------------------------------------------*/
    Button::EdgeConfig btnCfg;
    Button::EdgeTrigger button;

    btnCfg.gpioConfig    = pinCfg;
    btnCfg.activeEdge    = Button::ActiveEdge::RISING_EDGE;
    btnCfg.debounceTime  = DebounceTime;
    btnCfg.sampleRate    = SampleRate;
    btnCfg.stableSamples = 3;

    button.initialize( btnCfg );
    button.onActiveEdge( cb_user );
    button.enable();

    /*-------------------------------------------------
    Execute the test
    -------------------------------------------------*/
    uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "Testing debounced GPIO edge triggering functionality\r\n" );
    uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "Please press the button %d times\r\n", PressCount );

    size_t lastCount = 0;
    while ( NumPressed < PressCount )
    {
      if ( lastCount != NumPressed )
      {
        CHECK( NumPressed == ( lastCount + 1 ) );
        lastCount = NumPressed;
        uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "%d\r\n", NumPressed );
      }

      Chimera::delayMilliseconds( 5 );
      continue;
    }

    /*-------------------------------------------------
    Verification and cleanup
    -------------------------------------------------*/
    uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "Thank you!\r\n" );
    CHECK( NumPressed == PressCount );
    button.reset();
  }
}    // namespace Debounced