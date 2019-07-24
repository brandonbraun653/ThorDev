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

/* Boost Includes */
#include <boost/circular_buffer.hpp>

/* Chimera Includes */
#include <Chimera/system.hpp>
#include <Chimera/gpio.hpp>
#include <Chimera/threading.hpp>
#include <Chimera/serial.hpp>

#include <Chimera/types/event_types.hpp>

using namespace Chimera::GPIO;
using namespace Chimera::Serial;
using namespace Chimera::Threading;

static uint32_t testValue = 0;
static bool callbackExecuted = false;

static constexpr size_t CIRCULAR_BUFFER_SIZE = 100;
static constexpr size_t ARRAY_SIZE           = 100;

static boost::circular_buffer<uint8_t> txCircularBuffer;
static boost::circular_buffer<uint8_t> rxCircularBuffer;

static std::array<uint8_t, ARRAY_SIZE> txHardwareBuffer;
static std::array<uint8_t, ARRAY_SIZE> rxHardwareBuffer;

void blinkyThread( void *argument );
void serialThread( void *argument );
void testCallback( void *const handle, const size_t size );

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


#include <Thor/usart.hpp>
#include <Thor/gpio.hpp>

void serialThread( void *argument )
{
  using namespace Chimera::Hardware;
  using namespace Chimera::Serial;
  using namespace Thor::USART;

  signalSetupComplete();

  /*------------------------------------------------
  Memory Initialization
  ------------------------------------------------*/
  txCircularBuffer.resize( CIRCULAR_BUFFER_SIZE );
  rxCircularBuffer.resize( CIRCULAR_BUFFER_SIZE );

  txHardwareBuffer.fill( 0 );
  rxHardwareBuffer.fill( 0 );

  /*------------------------------------------------
  Callback Initialization
  ------------------------------------------------*/
  size_t callbackId = 700;
  Chimera::Event::Actionable callback;

  callback.element = std::bind( testCallback, std::placeholders::_1, std::placeholders::_2 );
  callback.trigger = Chimera::Event::Trigger::WRITE_COMPLETE;
  callback.type    = Chimera::Event::ElementType::CALLBACK;

  size_t callbackID1       = 750;
  SemaphoreHandle_t wakeup = xSemaphoreCreateBinary();

  Chimera::Event::Actionable callback1;
  callback1.element = wakeup;
  callback1.trigger = Chimera::Event::Trigger::WRITE_COMPLETE;
  callback1.type    = Chimera::Event::ElementType::THREAD_NOTIFIER;

  /*------------------------------------------------
  GPIO Initialization
  ------------------------------------------------*/
  Chimera::Serial::IOPins serialPins;

  serialPins.rx.alternate = Thor::Driver::GPIO::AF7_USART3;
  serialPins.rx.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  serialPins.rx.pin       = 11;
  serialPins.rx.port      = Chimera::GPIO::Port::PORTC;
  serialPins.rx.pull      = Chimera::GPIO::Pull::NO_PULL;

  serialPins.tx.alternate = Thor::Driver::GPIO::AF7_USART3;
  serialPins.tx.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  serialPins.tx.pin       = 10;
  serialPins.tx.port      = Chimera::GPIO::Port::PORTC;
  serialPins.tx.pull      = Chimera::GPIO::Pull::NO_PULL;

  /*------------------------------------------------
  USART Initialization
  ------------------------------------------------*/
  Thor::USART::USARTClass usart;
  Chimera::Serial::Config usartConfig;

  usartConfig.baud = 115200;
  usartConfig.flow = FlowControl::FCTRL_NONE;
  usartConfig.parity = Parity::PAR_NONE;
  usartConfig.stopBits = StopBits::SBITS_ONE;
  usartConfig.width    = CharWid::CW_8BIT;


  usart.assignHW( 3, serialPins );
  usart.configure( usartConfig );
  usart.begin( SubPeripheralMode::INTERRUPT, SubPeripheralMode::INTERRUPT );

  usart.enableBuffering( SubPeripheral::TX, &txCircularBuffer, txHardwareBuffer.data(), txHardwareBuffer.size() );

  usart.registerListener( callback, 100, callbackId );
  usart.registerListener( callback1, 100, callbackID1 );

  /*------------------------------------------------
  Test code
  ------------------------------------------------*/
  std::array<uint8_t, 5> readArray;
  readArray.fill( 0 );

  std::string startupString = "Hello! Feed me some bytes please :) \r\n";
  std::string allBytes      = "I got all the bytes!\r\n";
  std::string someBytes     = "I got some bytes, but aborted...\r\n";
  std::string callbackStr   = "Callback executed!\r\n";
  std::string bufferedStr   = "This is the queued string!\r\n";
  std::string bufferedStr2  = "Hey there person! :)\r\n";
  std::string bufferedStr3  = "Hey there person part 2! :)\r\n";

  usart.write( reinterpret_cast<const uint8_t*>( startupString.c_str() ), startupString.size(), 100 );
  usart.write( reinterpret_cast<const uint8_t*>( bufferedStr.c_str() ), bufferedStr.size(), 100 );
  usart.write( reinterpret_cast<const uint8_t*>( bufferedStr2.c_str() ), bufferedStr2.size(), 100 );
  usart.write( reinterpret_cast<const uint8_t*>( bufferedStr3.c_str() ), bufferedStr3.size(), 100 );

//  usart.receiveIT( readArray.data(), readArray.size(), 100 );

  while ( 1 )
  {
    
//    if ( usart.rxTransferStatus() == Chimera::Serial::Status::RX_COMPLETE )
//    {
//      usart.killReceive();
//      usart.transmitIT( reinterpret_cast<const uint8_t *>( allBytes.c_str() ), allBytes.size(), 100 );
//
//      while ( usart.txTransferStatus() != Chimera::Serial::Status::TX_COMPLETE )
//      {
//        Chimera::delayMilliseconds( 5 );
//      }
//
//      usart.transmitIT( readArray.data(), readArray.size(), 100 );
//    }
//
//    if ( usart.rxTransferStatus() == Chimera::Serial::Status::RX_ABORTED )
//    {
//      usart.transmitIT( reinterpret_cast<const uint8_t *>( someBytes.c_str() ), someBytes.size(), 100 );
//
//      usart.killReceive();
//    }
//
    if ( ( xSemaphoreTake( wakeup, portMAX_DELAY ) == pdPASS ) && ( testValue == 1 ) )
    {
      callbackExecuted = false;
      usart.write( reinterpret_cast<const uint8_t *>( callbackStr.c_str() ), callbackStr.size(), 100 );
    }
    
    Chimera::delayMilliseconds( 100 );
  }
}


void testCallback( void *const handle, const size_t size )
{
  testValue++;
  callbackExecuted = true;
}