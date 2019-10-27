/********************************************************************************
 *   File Name:
 *    main.cpp
 *
 *   Description:
 *    SPI development project entry
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/gpio.hpp>
#include <Chimera/spi.hpp>
#include <Chimera/system.hpp>
#include <Chimera/threading.hpp>
#include <Chimera/watchdog.hpp>

void test_thread( void *arguments );
void spi_thread( void *arguments );

void startup_sequence( Chimera::GPIO::GPIOClass *const led );

int main( void )
{
  Chimera::System::initialize();

  Chimera::Threading::addThread( test_thread, "wd_thread", 1000, nullptr, 3, nullptr );
  Chimera::Threading::addThread( spi_thread, "spi_thread", 1000, nullptr, 3, nullptr );
  Chimera::Threading::startScheduler();
  return 0;
}

void test_thread( void *arguments )
{
  Chimera::Threading::signalSetupComplete();

  Chimera::GPIO::PinInit ledInit;
  ledInit.accessMode = Chimera::Hardware::AccessMode::THREADED;
  ledInit.drive      = Chimera::GPIO::Drive::OUTPUT_PUSH_PULL;
  ledInit.port       = Chimera::GPIO::Port::PORTA;
  ledInit.pull       = Chimera::GPIO::Pull::NO_PULL;
  ledInit.pin        = 5;

  auto led = Chimera::GPIO::GPIOClass();
  led.init( ledInit );
  led.setState( Chimera::GPIO::State::HIGH );

  startup_sequence( &led );

  auto watchdog = Chimera::Watchdog::WatchdogClass();
  watchdog.initialize( 500, 100 );
  watchdog.start();

  while ( 1 )
  {
    watchdog.kick();
    led.setState( Chimera::GPIO::State::HIGH );
    Chimera::delayMilliseconds( 150 );
    led.setState( Chimera::GPIO::State::LOW );
    Chimera::delayMilliseconds( 150 );
  }
}

void spi_thread( void *arguments )
{
  Chimera::Threading::signalSetupComplete();

  std::string_view data = "Hello World!\r\n";


  Chimera::SPI::DriverConfig cfg;

  /*------------------------------------------------
  GPIO Initialization
  ------------------------------------------------*/
  cfg.CSInit.accessMode = Chimera::Hardware::AccessMode::THREADED;
  cfg.CSInit.alternate  = Thor::Driver::GPIO::AF_NONE;
  cfg.CSInit.drive      = Chimera::GPIO::Drive::OUTPUT_PUSH_PULL;
  cfg.CSInit.pin        = 2;
  cfg.CSInit.port       = Chimera::GPIO::Port::PORTD;
  cfg.CSInit.pull       = Chimera::GPIO::Pull::NO_PULL;

  cfg.SCKInit.accessMode = Chimera::Hardware::AccessMode::THREADED;
  cfg.SCKInit.alternate  = Thor::Driver::GPIO::AF7_SPI3;
  cfg.SCKInit.drive      = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  cfg.SCKInit.pin        = 10;
  cfg.SCKInit.port       = Chimera::GPIO::Port::PORTC;
  cfg.SCKInit.pull       = Chimera::GPIO::Pull::NO_PULL;

  cfg.MISOInit.accessMode = Chimera::Hardware::AccessMode::THREADED;
  cfg.MISOInit.alternate  = Thor::Driver::GPIO::AF7_SPI3;
  cfg.MISOInit.drive      = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  cfg.MISOInit.pin        = 11;
  cfg.MISOInit.port       = Chimera::GPIO::Port::PORTC;
  cfg.MISOInit.pull       = Chimera::GPIO::Pull::NO_PULL;

  cfg.MOSIInit.accessMode = Chimera::Hardware::AccessMode::THREADED;
  cfg.MOSIInit.alternate  = Thor::Driver::GPIO::AF7_SPI3;
  cfg.MOSIInit.drive      = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  cfg.MOSIInit.pin        = 12;
  cfg.MOSIInit.port       = Chimera::GPIO::Port::PORTC;
  cfg.MOSIInit.pull       = Chimera::GPIO::Pull::NO_PULL;

  /*------------------------------------------------
  SPI Parameter Initialization
  ------------------------------------------------*/
  cfg.HWInit.bitOrder    = Chimera::SPI::BitOrder::LSB_FIRST;
  cfg.HWInit.clockFreq   = 1000000;
  cfg.HWInit.clockMode   = Chimera::SPI::ClockMode::MODE0;
  cfg.HWInit.controlMode = Chimera::SPI::ControlMode::MASTER;
  cfg.HWInit.csMode      = Chimera::SPI::CSMode::MANUAL;
  cfg.HWInit.dataSize    = Chimera::SPI::DataSize::SZ_8BIT;
  cfg.HWInit.hwChannel   = 1;
  cfg.HWInit.txfrMode    = Chimera::SPI::TransferMode::BLOCKING;
  cfg.validity           = true;

  /*------------------------------------------------
  SPI Driver Initialization
  ------------------------------------------------*/
  auto spi = Chimera::SPI::SPIClass();

  if ( spi.init( cfg ) != Chimera::CommonStatusCodes::OK )
  {
    Chimera::Watchdog::invokeTimeout();
  }


  while ( 1 )
  {
    spi.writeBytes( data.data(), data.length(), 100 );
    spi.await( Chimera::Event::Trigger::TRANSFER_COMPLETE, 100 );

    Chimera::delayMilliseconds( 100 );
  }
}

void startup_sequence( Chimera::GPIO::GPIOClass *const led )
{
  for ( uint8_t i = 0; i < 5; i++ )
  {
    led->setState( Chimera::GPIO::State::HIGH );
    Chimera::delayMilliseconds( 65 );
    led->setState( Chimera::GPIO::State::LOW );
    Chimera::delayMilliseconds( 25 );
  }

  Chimera::delayMilliseconds( 350 );
}