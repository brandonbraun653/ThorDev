/********************************************************************************
 *  File Name:
 *    serial_output.cpp
 *
 *  Description:
 *    Insert Description
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Boost Includes */
#include <boost/circular_buffer.hpp>

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/gpio>
#include <Chimera/serial>
#include <Chimera/thread>

/* Testing Includes */
#include <tests/hardware/harness/serial_output.hpp>


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


namespace Thor::Testing
{
  void initializeSerial()
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
}    // namespace Thor::Testing
