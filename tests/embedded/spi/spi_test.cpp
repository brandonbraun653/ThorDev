/********************************************************************************
 *  File Name:
 *    system_flash_test.cpp
 *
 *  Description:
 *    Embedded tests for the Thor internal system flash driver 
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <algorithm>
#include <array>
#include <cstdint>

/* Boost Includes */
#include <boost/circular_buffer.hpp>

/* Test Driver Includes */
#include "TinyEmbeddedTest.h"

/* Chimera Includes */
#include "chimeraPort.hpp"
#include <Chimera/gpio.hpp>
#include <Chimera/spi.hpp>
#include <Chimera/serial.hpp>

static bool spi_initialized = false;
static bool ser_initialized = false;

Chimera::SPI::SPIClass spi;
Chimera::Serial::SerialClass serial;

std::array<uint8_t, 10> testTX = { 0xa1, 0xaa, 0x3a, 0x60, 0x59, 0x09, 0x90, 0xb2, 0x8a, 0x05 };

boost::circular_buffer<uint8_t> circle(15);
std::array<uint8_t, 15> internalbuffer;

/*------------------------------------------------
Local Functions
------------------------------------------------*/
void initialize()
{
  using namespace Chimera::GPIO;
  
  if ( !spi_initialized )
  {
    Chimera::SPI::Setup setup;

    setup.MOSI.port = Port::PORTC;
    setup.MOSI.pin  = 12;
    setup.MOSI.alternate = GPIO_AF6_SPI3;

    setup.MISO.port = Port::PORTC;
    setup.MISO.pin  = 11;
    setup.MISO.alternate = GPIO_AF6_SPI3;

    setup.SCK.port = Port::PORTC;
    setup.SCK.pin  = 10;
    setup.SCK.alternate = GPIO_AF6_SPI3;

    setup.CS.port = Port::PORTD;
    setup.CS.pin  = 2;

    setup.channel = 3;

    spi.init( setup );

    spi_initialized = true;
  }
}

void initializeSerial()
{
  using namespace Chimera::Serial;
  using namespace Chimera::GPIO;
  using namespace Chimera::Hardware;

  if ( !ser_initialized )
  {
    IOPins pins;
    pins.tx.port      = Port::PORTA;
    pins.tx.pin       = 0;
    pins.tx.alternate = GPIO_AF8_UART4;

    pins.rx.port      = Port::PORTA;
    pins.rx.pin       = 1;
    pins.rx.alternate = GPIO_AF8_UART4;

    serial.assignHW( 4, pins );
    serial.configure( 115200, CharWid::CW_8BIT, Parity::PAR_NONE, StopBits::SBITS_ONE, FlowControl::FCTRL_NONE );
    serial.enableBuffering(SubPeripheral::TX, &circle, internalbuffer.data(), internalbuffer.size());
    serial.begin( Modes::INTERRUPT, Modes::BLOCKING );
    ser_initialized = true;
  }
}

/*------------------------------------------------
Verify the memory helper functions
------------------------------------------------*/
TEST_GROUP( HelperFunctions ){};

/*------------------------------------------------
Verify the actual Flash memory interface behavior
------------------------------------------------*/
/* clang-format off */
TEST_GROUP( Sandbox )
{
  public:
    void teardown() final override
    {
      //reset_test();
    }
};
/* clang-format on */

TEST( Sandbox, t1 )
{
  using namespace Chimera::GPIO;
  using namespace Chimera::Hardware;
  using namespace Chimera::SPI;

  //  initialize();
  //
  //  spi.setPeripheralMode( SubPeripheral::TX, SubPeripheralMode::DMA );
  //  spi.setChipSelectControlMode( ChipSelectMode::AUTO_AFTER_TRANSFER );
  //  spi.writeBytes( testTX.data(), testTX.size(), 100 );
  //
  //  Chimera::delayMilliseconds( 1000 );
  //  Chimera::delayMilliseconds( 1 );

  initializeSerial();

  serial.write( testTX.data(), testTX.size() );
  Chimera::delayMilliseconds( 1000 );
  Chimera::delayMilliseconds( 1 );

  CHECK( true );
}
