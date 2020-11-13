/********************************************************************************
 *  File Name:
 *    test_entry.cpp
 *
 *  Description:
 *    Tests the HLD CAN bus driver on the target device
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

/* Testing Includes */
#include <tests/hardware/harness/serial_output.hpp>

/*-------------------------------------------------------------------------------
Static Declarations
-------------------------------------------------------------------------------*/
static void test_thread( void *argument );

/*-------------------------------------------------------------------------------
Static Data
-------------------------------------------------------------------------------*/
static std::array<char, 100> printBuffer;


/*-------------------------------------------------
CAN Driver Configuration
-------------------------------------------------*/
static std::array<Chimera::CAN::BasicFrame, 5> s_tx_buffer;
static std::array<Chimera::CAN::BasicFrame, 5> s_rx_buffer;

/*-------------------------------------------------------------------------------
Public Functions
-------------------------------------------------------------------------------*/
int main()
{
  using namespace Chimera::Threading;
  ChimeraInit();

  Thread testing;
  testing.initialize( test_thread, nullptr, Priority::LEVEL_3, STACK_KILOBYTES( 10 ), "test" );
  testing.start();

  startScheduler();
  return 0;
}


static void test_thread( void *argument )
{
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
  Filter Config
  -------------------------------------------------*/
  Chimera::CAN::Filter msgFilter;
  msgFilter.id       = 0x49;
  msgFilter.mask     = 0x00;
  msgFilter.extended = false;

  /*-------------------------------------------------
  Initialize
  -------------------------------------------------*/
  Chimera::Status_t initErr = Chimera::Status::OK;

  auto can = Chimera::CAN::getDriver( cfg.HWInit.channel );
  initErr |= can->open( cfg );
  initErr |= can->filter( &msgFilter, 1 );

  /*-------------------------------------------------------------------------------
  Perform the loopback test
  -------------------------------------------------------------------------------*/
  Chimera::CAN::BasicFrame rxFrame;
  Chimera::CAN::BasicFrame txFrame;

  txFrame.clear();
  txFrame.id         = 0x50;
  txFrame.idMode     = Chimera::CAN::IdType::STANDARD;
  txFrame.frameType  = Chimera::CAN::FrameType::DATA;
  txFrame.data[ 0 ]  = 0x33;
  txFrame.dataLength = 1;

  while ( 1 )
  {
    if ( ( initErr == Chimera::Status::OK ) && can->available() )
    {
      can->receive( rxFrame );
      Chimera::insert_debug_breakpoint();
    }
    else
    {
      can->send( txFrame );
    }

    Chimera::delayMilliseconds( 50 );
  }
}
