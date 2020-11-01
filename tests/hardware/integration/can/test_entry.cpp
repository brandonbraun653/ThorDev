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

/* Boost Includes */
#include <boost/circular_buffer.hpp>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/gpio>
#include <Chimera/serial>
#include <Chimera/thread>

/* Thor Includes */
#include <Thor/can>

/*-------------------------------------------------------------------------------
Constants
-------------------------------------------------------------------------------*/
static constexpr Chimera::Serial::Channel serialChannel = Chimera::Serial::Channel::SERIAL1;

/*-------------------------------------------------------------------------------
Static Declarations
-------------------------------------------------------------------------------*/
static void test_thread( void *argument );
static void initializeSerial();

/*-------------------------------------------------------------------------------
Static Data
-------------------------------------------------------------------------------*/
static std::array<char, 100> printBuffer;

/*-------------------------------------------------
Serial Driver Configuration
-------------------------------------------------*/
// Length of the hardware buffer for transceiving a Serial message
static constexpr size_t HWBufferSize = 128;

// Length of the user buffer for queueing multiple messages
static constexpr size_t CircularBufferSize = 2 * HWBufferSize;

// Serial Transmit Buffers
static std::array<uint8_t, HWBufferSize> sTXHWBuffer;
static boost::circular_buffer<uint8_t> sTXCircularBuffer( CircularBufferSize );

// Serial Recieve Buffers
static std::array<uint8_t, HWBufferSize> sRXHWBuffer;
static boost::circular_buffer<uint8_t> sRXCircularBuffer( CircularBufferSize );

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


static void initializeSerial()
{
  using namespace Chimera::Serial;
  using namespace Chimera::Hardware;

  /*------------------------------------------------
  Configuration info for the serial object
  ------------------------------------------------*/
  IOPins pins;
  pins.tx.alternate = Chimera::GPIO::Alternate::USART1_TX;
  pins.tx.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  pins.tx.pin       = 9;
  pins.tx.port      = Chimera::GPIO::Port::PORTA;
  pins.tx.pull      = Chimera::GPIO::Pull::NO_PULL;
  pins.tx.threaded  = true;
  pins.tx.validity  = true;

  pins.rx.alternate = Chimera::GPIO::Alternate::USART1_RX;
  pins.rx.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  pins.rx.pin       = 10;
  pins.rx.port      = Chimera::GPIO::Port::PORTA;
  pins.rx.pull      = Chimera::GPIO::Pull::NO_PULL;
  pins.rx.threaded  = true;
  pins.rx.validity  = true;


  Config cfg;
  cfg.baud     = 115200;
  cfg.flow     = FlowControl::FCTRL_NONE;
  cfg.parity   = Parity::PAR_NONE;
  cfg.stopBits = StopBits::SBITS_ONE;
  cfg.width    = CharWid::CW_8BIT;

  /*------------------------------------------------
  Create the serial object and initialize it
  ------------------------------------------------*/
  auto result = Chimera::Status::OK;
  auto Serial = Chimera::Serial::getDriver( serialChannel );

  if ( !Serial )
  {
    Chimera::insert_debug_breakpoint();
  }

  result |= Serial->assignHW( serialChannel, pins );
  result |= Serial->configure( cfg );
  result |= Serial->enableBuffering( SubPeripheral::TX, &sTXCircularBuffer, sTXHWBuffer.data(), sTXHWBuffer.size() );
  result |= Serial->enableBuffering( SubPeripheral::RX, &sRXCircularBuffer, sRXHWBuffer.data(), sRXHWBuffer.size() );
  result |= Serial->begin( PeripheralMode::INTERRUPT, PeripheralMode::INTERRUPT );
}


static void test_thread( void *argument )
{
  /*-------------------------------------------------
  Initialize HW Resources
  -------------------------------------------------*/
  initializeSerial();

  /*-------------------------------------------------
  Allocate the device drivers
  -------------------------------------------------*/
  auto serial = Chimera::Serial::getDriver( serialChannel );

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
  cfg.HWInit.baudRate           = 100000;
  cfg.HWInit.channel            = Chimera::CAN::Channel::CAN0;
  cfg.HWInit.maxBaudError       = 0.05f;
  cfg.HWInit.resyncJumpWidth    = 1;
  cfg.HWInit.samplePointPercent = 0.85f;
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
  msgFilter.mask     = 0xFF;
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

  while ( 1 )
  {
    if( ( initErr == Chimera::Status::OK ) && can->available() )
    {
      can->receive( rxFrame );
      Chimera::insert_debug_breakpoint();
    }

    Chimera::delayMilliseconds( 15 );
  }
}
