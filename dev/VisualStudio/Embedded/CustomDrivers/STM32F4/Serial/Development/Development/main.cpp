/********************************************************************************
 *   File Name:
 *       main.cpp
 *
 *   Description:
 *       Entry point for embedded development of the custom Serial (USART/UART)
 *       driver for the STM32F4 series of devices.
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

 /* C++ Includes */
 #include <functional>


/* Chimera Includes */
#include <Chimera/system.hpp>
#include <Chimera/gpio.hpp>
#include <Chimera/threading.hpp>
#include <Chimera/serial.hpp>

#include <Chimera/types/callback_types.hpp>

using namespace Chimera::GPIO;
using namespace Chimera::Serial;
using namespace Chimera::Threading;

void blinkyThread( void *argument );
void serialThread( void *argument );
void testCallback( void *handle, size_t data );

int main()
{
  Chimera::System::initialize();

  addThread( blinkyThread, "blinky", 500, nullptr, 2, nullptr );
  addThread( serialThread, "serial", 1500, nullptr, 2, nullptr );

  startScheduler();

  while ( 1 ) {}
  return 0;
}

void blinkyThread( void *argument )
{
  GPIOClass gpio;
  PinInit init;

  init.drive      = Drive::OUTPUT_PUSH_PULL;
  init.pin        = 5;
  init.port       = Port::PORTA;
  init.pull       = Pull::NO_PULL;
  init.state      = State::LOW;
  init.accessMode = Chimera::Hardware::AccessMode::THREADED;

  gpio.init( init );

  signalSetupComplete();

  while ( 1 )
  {
    gpio.setState( State::HIGH );
    Chimera::delayMilliseconds( 500 );

    gpio.setState( State::LOW );
    Chimera::delayMilliseconds( 500 );
  }
}

#include <Thor/drivers/common/types/serial_types.hpp>
#include <Thor/drivers/f4/rcc/hw_rcc_driver.hpp>
#include <Thor/drivers/f4/usart/hw_usart_driver.hpp>

void serialThread( void *argument )
{
  using namespace Thor::Driver;
  using namespace Thor::Driver::Serial;
  using namespace Thor::Driver::RCC;

  signalSetupComplete();



  GPIOClass rxPin;
  PinInit rxInit;

  rxInit.alternate = GPIO::AF7_USART3;
  rxInit.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  rxInit.pin       = 11;
  rxInit.port      = Chimera::GPIO::Port::PORTC;
  rxInit.pull      = Chimera::GPIO::Pull::NO_PULL;

  rxPin.init( rxInit );

  GPIOClass txPin;
  PinInit txInit;

  txInit.alternate = GPIO::AF7_USART3;
  txInit.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  txInit.pin       = 10;
  txInit.port      = Chimera::GPIO::Port::PORTC;
  txInit.pull      = Chimera::GPIO::Pull::NO_PULL;

  txPin.init( txInit );

  Config cfg;
  cfg.BaudRate   = 115200;
  cfg.Mode       = USART::Configuration::Modes::TX_RX;
  cfg.Parity     = USART::Configuration::Parity::NONE;
  cfg.StopBits   = USART::Configuration::Stop::BIT_1;
  cfg.WordLength = USART::Configuration::WordLength::LEN_8BIT;

  Chimera::Callback::ISRCallback cbHandle;
  cbHandle.func = std::bind( testCallback, std::placeholders::_1, std::placeholders::_2 );

  
  USART::Driver usart( USART::USART3_PERIPH );

  usart.init( cfg );
  usart.enableIT( Chimera::Hardware::SubPeripheral::TX );
  usart.attachCallback( Chimera::Event::Trigger::WRITE_COMPLETE, cbHandle );

  std::array<uint8_t, 5> readArray;
  readArray.fill( 0 );

  std::string startupString = "Hello! Feed me some bytes please :) \r\n";
  std::string allBytes      = "I got all the bytes!\r\n";
  std::string someBytes     = "I got some bytes, but aborted...\r\n";

  usart.transmitIT( reinterpret_cast<const uint8_t*>( startupString.c_str() ), startupString.size(), 100 );
  usart.receiveIT( readArray.data(), readArray.size(), 100 );



  while ( 1 )
  {
    
    if ( usart.rxTransferStatus() == Chimera::Serial::Status::RX_COMPLETE )
    {
      usart.killReceive();
      usart.transmitIT( reinterpret_cast<const uint8_t *>( allBytes.c_str() ), allBytes.size(), 100 );

      while ( usart.txTransferStatus() != Chimera::Serial::Status::TX_COMPLETE )
      {
        Chimera::delayMilliseconds( 5 );
      }

      usart.transmitIT( readArray.data(), readArray.size(), 100 );
    }

    if ( usart.rxTransferStatus() == Chimera::Serial::Status::RX_ABORTED )
    {
      usart.transmitIT( reinterpret_cast<const uint8_t *>( someBytes.c_str() ), someBytes.size(), 100 );

      usart.killReceive();
    }

    Chimera::delayMilliseconds( 100 );
  }
}


void testCallback( void *handle, size_t data )
{
  // Do nothing for now...just checking the bind
}