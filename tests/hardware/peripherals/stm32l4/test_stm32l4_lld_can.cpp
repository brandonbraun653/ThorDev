/********************************************************************************
 *  File Name:
 *    test_stm32l4_lld_can.cpp
 *
 *  Description:
 *    Tests the LLD CAN bus driver for the STM32L4 port
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
#include <Thor/lld/interface/can/can_intf.hpp>

/* CppUTest Includes  */
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>

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
  Configure the output style:
    - Colorized
    - Verbose
  -------------------------------------------------*/
  const char *av_override[] = { "-c", "-v" };

  /*-------------------------------------------------
  Initialize HW Resources
  -------------------------------------------------*/
  initializeSerial();

  /*-------------------------------------------------
  Allocate the device drivers
  -------------------------------------------------*/
  auto serial = Chimera::Serial::getDriver( serialChannel );

  /*-------------------------------------------------
  Run the tests then break
  -------------------------------------------------*/
  snprintf(printBuffer.data(), printBuffer.size(), "Starting STM32L4 LLD CAN Bus Tests\n" );
  serial->lock();
  serial->write( printBuffer.data(), strlen( printBuffer.data() ) );
  serial->await( Chimera::Event::Trigger::TRIGGER_WRITE_COMPLETE, Chimera::Threading::TIMEOUT_BLOCK );
  serial->unlock();

  int rcode = CommandLineTestRunner::RunAllTests( 2, av_override );

  snprintf(printBuffer.data(), printBuffer.size(), "Test exit with code: %d\n", rcode );
  serial->lock();
  serial->write( printBuffer.data(), strlen( printBuffer.data() ) );
  serial->await( Chimera::Event::Trigger::TRIGGER_WRITE_COMPLETE, Chimera::Threading::TIMEOUT_BLOCK );
  serial->unlock();

  while ( 1 )
  {
    Chimera::insert_debug_breakpoint();
  }
}

/*-------------------------------------------------------------------------------
Testing Functions
-------------------------------------------------------------------------------*/
TEST_GROUP( STM32L4_LLD_CAN ){};

TEST( STM32L4_LLD_CAN, Open )
{
  CHECK_EQUAL( 1, 1 );
}
