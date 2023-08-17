/********************************************************************************
 *  File Name:
 *    test_rotary_encoder.cpp
 *
 *  Description:
 *    Tests the SMT32 HLD GPIO when used as a rotary encoder processor
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
static const Chimera::GPIO::PinInit cfgCenterButton = { .alternate = Chimera::GPIO::Alternate::NONE,
                                                        .drive     = Chimera::GPIO::Drive::INPUT,
                                                        .pin       = 1,
                                                        .port      = Chimera::GPIO::Port::PORTA,
                                                        .pull      = Chimera::GPIO::Pull::NO_PULL,
                                                        .state     = Chimera::GPIO::State::HIGH,
                                                        .threaded  = true,
                                                        .validity  = true };

static const Chimera::GPIO::PinInit cfgEncoderA = { .alternate = Chimera::GPIO::Alternate::NONE,
                                                    .drive     = Chimera::GPIO::Drive::INPUT,
                                                    .pin       = 0,
                                                    .port      = Chimera::GPIO::Port::PORTB,
                                                    .pull      = Chimera::GPIO::Pull::NO_PULL,
                                                    .state     = Chimera::GPIO::State::HIGH,
                                                    .threaded  = true,
                                                    .validity  = true };

static const Chimera::GPIO::PinInit cfgEncoderB = { .alternate = Chimera::GPIO::Alternate::NONE,
                                                    .drive     = Chimera::GPIO::Drive::INPUT,
                                                    .pin       = 2,
                                                    .port      = Chimera::GPIO::Port::PORTA,
                                                    .pull      = Chimera::GPIO::Pull::NO_PULL,
                                                    .state     = Chimera::GPIO::State::HIGH,
                                                    .threaded  = true,
                                                    .validity  = true };


TEST_GROUP( STM32_HLD_GPIO_ENCODER ){};

/*-------------------------------------------------------------------------------
Center Button Pressing
-------------------------------------------------------------------------------*/
namespace CenterButton
{
  /*-------------------------------------------------
  Constants
  -------------------------------------------------*/
  static constexpr size_t DebounceTime = 200;
  static constexpr size_t SampleRate   = 10;

  /*-------------------------------------------------
  Variables
  -------------------------------------------------*/
  static volatile size_t NumPressed = 0;

  static void usercallback( Aurora::HMI::Button::ActiveEdge edge )
  {
    NumPressed++;
  }
  static auto cb_user = Aurora::HMI::Button::EdgeCallback::create<usercallback>();


  TEST( STM32_HLD_GPIO_ENCODER, CenterPress )
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
    RotaryEncoder::REConfig btnCfg;
    RotaryEncoder::Encoder button;

    btnCfg.idleHigh          = true;
    btnCfg.pinPushActiveEdge = Button::ActiveEdge::RISING_EDGE;
    btnCfg.debounceTime      = DebounceTime;
    btnCfg.sampleRate        = SampleRate;
    btnCfg.stableSamples     = 3;
    btnCfg.pinACfg           = cfgEncoderA;
    btnCfg.pinBCfg           = cfgEncoderB;
    btnCfg.pinPushCfg        = cfgCenterButton;

    button.initialize( btnCfg );
    button.enable();
    button.onCenterPush( cb_user );

    /*-------------------------------------------------
    Execute the test
    -------------------------------------------------*/
    uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "Please press the center button %d times\r\n", PressCount );

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
}    // namespace CenterButton


/*-------------------------------------------------------------------------------
Rotary Tests
-------------------------------------------------------------------------------*/
namespace Encoder
{
  /*-------------------------------------------------
  Constants
  -------------------------------------------------*/
  static constexpr size_t DebounceTime = 200;
  static constexpr size_t SampleRate   = 10;

  /*-------------------------------------------------
  Variables
  -------------------------------------------------*/
  static volatile int currentRotation = 0;

  static void usercallback( int num_clicks )
  {
    currentRotation += num_clicks;
  }
  static auto cb_user = Aurora::HMI::RotaryEncoder::RotationCallback::create<usercallback>();


  TEST( STM32_HLD_GPIO_ENCODER, Rotation )
  {
    using namespace Aurora::HMI;
    constexpr int MinRotation = -20;
    constexpr int MaxRotation = 20;

    /*-------------------------------------------------
    Initialize the ISR data
    -------------------------------------------------*/
    currentRotation = 0;

    /*-------------------------------------------------
    Initialize the debouncing driver
    -------------------------------------------------*/
    RotaryEncoder::REConfig encCfg;
    RotaryEncoder::Encoder encoder;

    encCfg.idleHigh          = false;
    encCfg.pinPushActiveEdge = Button::ActiveEdge::RISING_EDGE;
    encCfg.pinAActiveEdge    = Button::ActiveEdge::BOTH_EDGES;
    encCfg.debounceTime      = DebounceTime;
    encCfg.sampleRate        = SampleRate;
    encCfg.stableSamples     = 2;
    encCfg.pinACfg           = cfgEncoderA;
    encCfg.pinBCfg           = cfgEncoderB;
    encCfg.pinPushCfg        = cfgCenterButton;

    encoder.initialize( encCfg );
    encoder.enable();
    encoder.onRotation( cb_user );

    /*-------------------------------------------------
    Execute the test
    -------------------------------------------------*/
    uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "Please rotate through either %d or %d\r\n", MinRotation, MaxRotation );

    int lastCount = 0;
    while ( ( MinRotation < currentRotation ) && ( currentRotation < MaxRotation ) )
    {
      if ( lastCount != currentRotation )
      {
        lastCount = currentRotation;
        uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "%d\r\n", currentRotation );
      }

      Chimera::delayMilliseconds( 5 );
      continue;
    }

    /*-------------------------------------------------
    Verification and cleanup
    -------------------------------------------------*/
    uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "Thank you!\r\n" );
    encoder.reset();
  }
}    // namespace Encoder
