/********************************************************************************
 *  File Name:
 *    test_hld_can_hw.cpp
 *
 *  Description:
 *    Tests the HLD CAN bus driver on the target device. This will send a
 *    predetermined sequence of messages back and forth between a PC and the DUT
 *    that exercises the full suite of Transmit and Receive data types associated
 *    with the CAN bus driver.
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <array>
#include <cstring>
#include <memory>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/gpio>
#include <Chimera/serial>
#include <Chimera/thread>

/* Thor Includes */
#include <Thor/can>
#include <Thor/lld/interface/can/can_intf.hpp>

/* Testing Includes */
#include <tests/hardware/harness/blinky.hpp>
#include <tests/hardware/harness/logger.hpp>

/*-------------------------------------------------------------------------------
Static Functions & Data
-------------------------------------------------------------------------------*/
static void test_thread( void *argument );
static void init_test_data();

/*-------------------------------------------------
CAN Driver Configuration
-------------------------------------------------*/
static std::array<Chimera::CAN::BasicFrame, 5> s_tx_buffer;
static std::array<Chimera::CAN::BasicFrame, 5> s_rx_buffer;

/*-------------------------------------------------
Transmit frames are sent out to the PC and expect a
response. Each RX frame has it's own filter.
-------------------------------------------------*/
static constexpr size_t NUM_TEST_CASES = 2;
static std::array<Chimera::CAN::BasicFrame, NUM_TEST_CASES> s_test_tx_frames;
static std::array<Chimera::CAN::BasicFrame, NUM_TEST_CASES> s_test_rx_frames;
static std::array<Chimera::CAN::Filter, NUM_TEST_CASES> s_test_rx_filters;

/*-------------------------------------------------------------------------------
Public Functions
-------------------------------------------------------------------------------*/
int main()
{
  using namespace Chimera::Threading;
  ChimeraInit();

  /*-------------------------------------------------
  Create the blinky thread
  -------------------------------------------------*/
  Thread blinky;
  blinky.initialize( Thor::Testing::blinkyThread, nullptr, Priority::LEVEL_1, Thor::Testing::BLINKY_STACK,
                     Thor::Testing::BLINKY_NAME.cbegin() );
  blinky.start();

  /*-------------------------------------------------
  Create the test thread
  -------------------------------------------------*/
  Thread testing;
  testing.initialize( test_thread, nullptr, Priority::LEVEL_3, STACK_KILOBYTES( 10 ), "test" );
  testing.start();

  startScheduler();
  return 0;
}


static void test_thread( void *argument )
{
  /*-------------------------------------------------
  Initialize the Logger
  -------------------------------------------------*/
  Thor::Testing::initializeLogger( uLog::Level::LVL_DEBUG );

  /*-------------------------------------------------------------------------------
  Initialize the CAN bus driver
  -------------------------------------------------------------------------------*/
  Chimera::CAN::DriverConfig cfg;
  cfg.clear();
  cfg.validity = true;

  /*-------------------------------------------------
  GPIO Configuration
  -------------------------------------------------*/
  cfg.TXInit.alternate = Chimera::GPIO::Alternate::CAN1_TX;
  cfg.TXInit.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  cfg.TXInit.pin       = 12;
  cfg.TXInit.port      = Chimera::GPIO::Port::PORTA;
  cfg.TXInit.pull      = Chimera::GPIO::Pull::NO_PULL;
  cfg.TXInit.threaded  = true;
  cfg.TXInit.validity  = true;

  cfg.RXInit.alternate = Chimera::GPIO::Alternate::CAN1_RX;
  cfg.RXInit.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  cfg.RXInit.pin       = 11;
  cfg.RXInit.port      = Chimera::GPIO::Port::PORTA;
  cfg.RXInit.pull      = Chimera::GPIO::Pull::NO_PULL;
  cfg.RXInit.threaded  = true;
  cfg.RXInit.validity  = true;

  /*-------------------------------------------------
  CAN Parameter Configuration
  -------------------------------------------------*/
  cfg.HWInit.baudRate           = 500000;
  cfg.HWInit.channel            = Chimera::CAN::Channel::CAN0;
  cfg.HWInit.maxBaudError       = 5.0f;
  cfg.HWInit.resyncJumpWidth    = 1;
  cfg.HWInit.samplePointPercent = 0.875f;
  cfg.HWInit.timeQuanta         = 16;
  cfg.HWInit.rxBuffer           = s_rx_buffer.data();
  cfg.HWInit.rxElements         = s_rx_buffer.size();
  cfg.HWInit.txBuffer           = s_tx_buffer.data();
  cfg.HWInit.txElements         = s_tx_buffer.size();

  /*-------------------------------------------------
  CAN initialize the test data
  -------------------------------------------------*/
  init_test_data();

  /*-------------------------------------------------
  CAN Enable
  -------------------------------------------------*/
  Chimera::Status_t runtimeError = Chimera::Status::OK;

  auto can = Chimera::CAN::getDriver( cfg.HWInit.channel );
  runtimeError |= can->open( cfg );
  runtimeError |= can->filter( s_test_rx_filters.data(), s_test_rx_filters.size() );

  /*-------------------------------------------------------------------------------
  Perform the test suite
  -------------------------------------------------------------------------------*/
  int errorCode = 0;
  uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "Starting CAN comms test\r\n" );

  if ( runtimeError != Chimera::Status::OK )
  {
    uLog::getRootSink()->flog( uLog::Level::LVL_ERROR, "Failed to init CAN driver\r\n" );
    errorCode = -1;
  }
  else
  {
    /*-------------------------------------------------
    Basic test setup
    -------------------------------------------------*/
    constexpr size_t RX_TIMEOUT = 3 * Chimera::Threading::TIMEOUT_1S;
    size_t lastTick             = Chimera::millis();
    bool timeoutOccurred        = false;

    /*-------------------------------------------------
    Send a dummy frame to clue the PC into listening.
    Some weird corruption happens on the CAN bus if the
    test jumps straight to the real data.
    -------------------------------------------------*/
    Chimera::CAN::BasicFrame syncFrame;
    syncFrame.clear();
    syncFrame.id         = 0x22;
    syncFrame.idMode     = Chimera::CAN::IdType::STANDARD;
    syncFrame.frameType  = Chimera::CAN::FrameType::DATA;
    syncFrame.data[ 0 ]  = 0xFF;
    syncFrame.dataLength = 1;

    runtimeError |= can->send( syncFrame );
    Chimera::delayMilliseconds( 500 );

    /*-------------------------------------------------
    Execute the test
    -------------------------------------------------*/
    Chimera::CAN::BasicFrame rxFrame;

    for ( auto test = 0; test < NUM_TEST_CASES; test++ )
    {
      /*-------------------------------------------------
      Make sure no errors have occurred before proceeding
      -------------------------------------------------*/
      if ( runtimeError != Chimera::Status::OK )
      {
        uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "Test step [%d] failed!\n", test - 1 );
        break;
      }

      /*-------------------------------------------------
      Kick off the test by sending the appropriate frame
      -------------------------------------------------*/
      runtimeError |= can->send( s_test_tx_frames[ test ] );
      lastTick = Chimera::millis();

      /*-------------------------------------------------
      Wait for the response from the PC
      -------------------------------------------------*/
      while ( ( Chimera::millis() - lastTick ) < RX_TIMEOUT )
      {
        if ( ( runtimeError == Chimera::Status::OK ) && can->available() )
        {
          /*-------------------------------------------------
          Capture the frame that was received
          -------------------------------------------------*/
          rxFrame.clear();
          runtimeError |= can->receive( rxFrame );

          /*-------------------------------------------------
          Does it match the expected frame?
          -------------------------------------------------*/
          if ( rxFrame != s_test_rx_frames[ test ] )
          {
            uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "Test step [%d] did not receive the expected frame\r\n", test );
            runtimeError = Chimera::Status::FAILED_READ;
            errorCode    = -2;
          }

          /*-------------------------------------------------
          Cleanly exit the loop
          -------------------------------------------------*/
          timeoutOccurred = false;
          break;
        }

        Chimera::delayMilliseconds( 15 );
        timeoutOccurred = true;
      }

      /*-------------------------------------------------
      Fail on no-receive
      -------------------------------------------------*/
      if ( timeoutOccurred )
      {
        runtimeError = Chimera::Status::TIMEOUT;
        uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "Test step [%d] timed out\r\n", test );
        errorCode = -3;
      }
    }
  }

  /*-------------------------------------------------------------------------------
  Idle away into nothing
  -------------------------------------------------------------------------------*/
  uLog::getRootSink()->flog( uLog::Level::LVL_INFO, "Testing exited with error code: %d\r\n", errorCode );
  while ( 1 )
  {
    Chimera::delayMilliseconds( 100 );
  }
}

/*-------------------------------------------------------------------------------
Static Function Definition
-------------------------------------------------------------------------------*/
static void init_test_data()
{
  size_t idx = 0;

  /*-------------------------------------------------
  Test Case: Standard ID Echo
  -------------------------------------------------*/
  s_test_tx_frames[ idx ].clear();
  s_test_tx_frames[ idx ].id         = 0x50;
  s_test_tx_frames[ idx ].idMode     = Chimera::CAN::IdType::STANDARD;
  s_test_tx_frames[ idx ].frameType  = Chimera::CAN::FrameType::DATA;
  s_test_tx_frames[ idx ].data[ 0 ]  = 0x33;
  s_test_tx_frames[ idx ].dataLength = 1;

  s_test_rx_frames[ idx ].clear();
  s_test_rx_frames[ idx ].id         = 0x50;
  s_test_rx_frames[ idx ].idMode     = Chimera::CAN::IdType::STANDARD;
  s_test_rx_frames[ idx ].frameType  = Chimera::CAN::FrameType::DATA;
  s_test_rx_frames[ idx ].data[ 0 ]  = 0x33;
  s_test_rx_frames[ idx ].dataLength = 1;

  s_test_rx_filters[ idx ].clear();
  s_test_rx_filters[ idx ].id       = 0x50;
  s_test_rx_filters[ idx ].mask     = 0x50;
  s_test_rx_filters[ idx ].extended = false;

  idx++;

  /*-------------------------------------------------
  Test Case: Extended ID Echo
  -------------------------------------------------*/
  s_test_tx_frames[ idx ].clear();
  s_test_tx_frames[ idx ].id         = 0x43f550;
  s_test_tx_frames[ idx ].idMode     = Chimera::CAN::IdType::EXTENDED;
  s_test_tx_frames[ idx ].frameType  = Chimera::CAN::FrameType::DATA;
  s_test_tx_frames[ idx ].data[ 0 ]  = 0xaa;
  s_test_tx_frames[ idx ].dataLength = 1;

  s_test_rx_frames[ idx ].clear();
  s_test_rx_frames[ idx ].id         = 0x43f550;
  s_test_rx_frames[ idx ].idMode     = Chimera::CAN::IdType::EXTENDED;
  s_test_rx_frames[ idx ].frameType  = Chimera::CAN::FrameType::DATA;
  s_test_rx_frames[ idx ].data[ 0 ]  = 0xaa;
  s_test_rx_frames[ idx ].dataLength = 1;

  s_test_rx_filters[ idx ].clear();
  s_test_rx_filters[ idx ].id       = 0x43f550;
  s_test_rx_filters[ idx ].mask     = 0x43f550;
  s_test_rx_filters[ idx ].extended = true;

  idx++;
}