/********************************************************************************
 *   File Name:
 *       main.cpp
 *
 *   Description:
 *       Entry for hardware development and tests
 *
 *   2020 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <string>

/* Chimera Includes */
#include <Chimera/clock>
#include <Chimera/gpio>
#include <Chimera/pwm>
#include <Chimera/spi>
#include <Chimera/system>
#include <Chimera/thread>
#include <Chimera/timer>
//#include <Chimera/watchdog>

/* Thor Includes */
#include <Thor/timer>

///* Logger Includes */
//#include <uLog/ulog.hpp>
//#include <uLog/sinks/sink_cout.hpp>
//#include <uLog/sinks/sink_vgdb_semihosting.hpp>
//
///* Visual GDB Includes */
//#include "SysprogsProfiler.h"

using namespace Chimera::Clock;
using namespace Chimera::Threading;

static void background_thread( void *arg );
static void startup_blinky_sequence( const Chimera::GPIO::GPIO_sPtr &led );

static void spi_thread( void *arg );
static void pwm_thread( void *arg );

int main( void )
{
  /*------------------------------------------------
  Boot up the device hardware
  ------------------------------------------------*/
  ChimeraInit();

  Chimera::System::Information *sysInfo = nullptr;
  Chimera::System::getSystemInformation( sysInfo );

  volatile size_t sysclk = Chimera::Clock::getFrequency( Bus::MSI );

  volatile size_t pclk1Freq = Chimera::Clock::getFrequency( Bus::PCLK1 );
  volatile size_t pclk2Freq = Chimera::Clock::getFrequency( Bus::PCLK2 );


  /*------------------------------------------------
  Initialize the profiler
  ------------------------------------------------*/
  //    InitializeInstrumentingProfiler();

  /*------------------------------------------------
  Initialize the logger system
  ------------------------------------------------*/
  //    uLog::initialize();
  //
  //    uLog::SinkHandle rootSink = std::make_shared<uLog::VGDBSemihostingSink>();
  //    rootSink->setLogLevel( uLog::Level::LVL_DEBUG );
  //    rootSink->setName( "Root" );
  //    rootSink->enable();
  //
  //    uLog::registerSink( rootSink );
  //    uLog::setGlobalLogLevel( uLog::Level::LVL_DEBUG );
  //    uLog::setRootSink( rootSink );
  //
  //    rootSink->flog( uLog::Level::LVL_INFO, "Boot up the world!\n" );

  /*------------------------------------------------
  Create the system threads
  ------------------------------------------------*/
  Thread blinkyThread;
  blinkyThread.initialize( background_thread, nullptr, Priority::LEVEL_3, 500, "blinky" );
  blinkyThread.start();

  Thread spiThread;
  spiThread.initialize( spi_thread, nullptr, Priority::LEVEL_4, 2048, "spi" );
  spiThread.start();

  Thread pwmThread;
  pwmThread.initialize( pwm_thread, nullptr, Priority::LEVEL_4, 2048, "pwm" );
  pwmThread.start();

  startScheduler();
}

void startup_blinky_sequence( const Chimera::GPIO::GPIO_sPtr &led )
{
  for ( uint8_t i = 0; i < 5; i++ )
  {
    led->setState( Chimera::GPIO::State::HIGH, 100 );
    Chimera::delayMilliseconds( 65 );
    led->setState( Chimera::GPIO::State::LOW, 100 );
    Chimera::delayMilliseconds( 25 );
  }

  Chimera::delayMilliseconds( 350 );
}

void background_thread( void *arguments )
{
  /*------------------------------------------------
  Initialize the LED gpio
  ------------------------------------------------*/
  Chimera::GPIO::PinInit ledInit;
  ledInit.threaded = false;
  ledInit.validity = true;
  ledInit.drive    = Chimera::GPIO::Drive::OUTPUT_PUSH_PULL;
  ledInit.port     = Chimera::GPIO::Port::PORTB;
  ledInit.pull     = Chimera::GPIO::Pull::NO_PULL;
  ledInit.pin      = 3;

  auto led = Chimera::GPIO::create_shared_ptr();
  led->init( ledInit, TIMEOUT_DONT_WAIT );
  led->setState( Chimera::GPIO::State::HIGH, TIMEOUT_DONT_WAIT );

  startup_blinky_sequence( led );

  while ( 1 )
  {
    led->setState( Chimera::GPIO::State::HIGH, 100 );
    Chimera::delayMilliseconds( 150 );
    led->setState( Chimera::GPIO::State::LOW, 100 );
    Chimera::delayMilliseconds( 150 );
  }
}

void spi_thread( void *arg )
{
  /*------------------------------------------------
  Initialize the SPI driver
  ------------------------------------------------*/
  Chimera::SPI::DriverConfig init;

  /* Chip select setup */
  init.externalCS = false;
  init.validity   = true;

  init.CSInit.alternate = Chimera::GPIO::Alternate::NONE;
  init.CSInit.drive     = Chimera::GPIO::Drive::OUTPUT_PUSH_PULL;
  init.CSInit.pin       = 4;
  init.CSInit.port      = Chimera::GPIO::Port::PORTA;
  init.CSInit.pull      = Chimera::GPIO::Pull::NO_PULL;
  init.CSInit.state     = Chimera::GPIO::State::HIGH;
  init.CSInit.threaded  = false;
  init.CSInit.validity  = true;

  init.SCKInit.alternate = Chimera::GPIO::Alternate::SPI1_SCK;
  init.SCKInit.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  init.SCKInit.pin       = 5;
  init.SCKInit.port      = Chimera::GPIO::Port::PORTA;
  init.SCKInit.threaded  = false;
  init.SCKInit.validity  = true;

  init.MISOInit.alternate = Chimera::GPIO::Alternate::SPI1_MISO;
  init.MISOInit.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  init.MISOInit.pin       = 6;
  init.MISOInit.port      = Chimera::GPIO::Port::PORTA;
  init.MISOInit.threaded  = false;
  init.MISOInit.validity  = true;

  init.MOSIInit.alternate = Chimera::GPIO::Alternate::SPI1_MOSI;
  init.MOSIInit.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  init.MOSIInit.pin       = 7;
  init.MOSIInit.port      = Chimera::GPIO::Port::PORTA;
  init.MOSIInit.threaded  = false;
  init.MOSIInit.validity  = true;

  init.HWInit.bitOrder    = Chimera::SPI::BitOrder::MSB_FIRST;
  init.HWInit.clockFreq   = 1000000;
  init.HWInit.clockMode   = Chimera::SPI::ClockMode::MODE0;
  init.HWInit.controlMode = Chimera::SPI::ControlMode::MASTER;
  init.HWInit.csMode      = Chimera::SPI::CSMode::AUTO_AFTER_TRANSFER;
  init.HWInit.dataSize    = Chimera::SPI::DataSize::SZ_8BIT;
  init.HWInit.hwChannel   = Chimera::SPI::Channel::SPI1;
  init.HWInit.txfrMode    = Chimera::Hardware::PeripheralMode::INTERRUPT;
  init.HWInit.validity    = true;

  auto spi = Chimera::SPI::create_shared_ptr();

  volatile auto result = spi->init( init );

  std::string_view test = "Hello World\r\n";

  while ( 1 )
  {
    spi->writeBytes( test.cbegin(), test.length(), Chimera::Threading::TIMEOUT_DONT_WAIT );
    Chimera::delayMilliseconds( 100 );
  }
}

void pwm_thread( void *arg )
{
  
  auto red = Chimera::PWM::create_shared_ptr();
  auto green = Chimera::PWM::create_shared_ptr();
  auto blue = Chimera::PWM::create_shared_ptr();
  
  Chimera::PWM::DriverConfig init;

  /*------------------------------------------------
  Initialize the Red LED Channel
  ------------------------------------------------*/
  init.timer.countDirection = Chimera::Timer::Direction::COUNT_UP;
  init.timer.overwrite      = true;
  init.timer.peripheral     = Chimera::Timer::Peripheral::TIMER2;
  init.timer.reloadValue    = 1000;
  init.timer.prescaler      = 80;
  init.timer.validity       = true;

  init.pwm.outputChannel = Chimera::Timer::Channel::CHANNEL_1;
  init.pwm.compareMatch  = 100;
  init.pwm.polarity      = Chimera::Timer::PWM::Polarity::ACTIVE_HIGH;
  init.pwm.mode          = Chimera::Timer::PWM::Mode::EDGE_ALIGNED;
  init.pwm.validity      = true;

  init.outputPin.pin       = 0;
  init.outputPin.port      = Chimera::GPIO::Port::PORTA;
  init.outputPin.alternate = Chimera::GPIO::Alternate::TIM2_CH1;
  init.outputPin.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  init.outputPin.threaded  = false;
  init.outputPin.validity  = true;
  
  init.validity = true;

  red->init( init );
  red->enableOutput();

  /*------------------------------------------------
  Initialize the Green LED Channel
  ------------------------------------------------*/
  init.timer.countDirection = Chimera::Timer::Direction::COUNT_UP;
  init.timer.overwrite      = true;
  init.timer.peripheral     = Chimera::Timer::Peripheral::TIMER2;
  init.timer.reloadValue    = 1000;
  init.timer.prescaler      = 80;
  init.timer.validity       = true;

  init.pwm.outputChannel = Chimera::Timer::Channel::CHANNEL_2;
  init.pwm.compareMatch  = 100;
  init.pwm.polarity      = Chimera::Timer::PWM::Polarity::ACTIVE_HIGH;
  init.pwm.mode          = Chimera::Timer::PWM::Mode::EDGE_ALIGNED;
  init.pwm.validity      = true;

  init.outputPin.pin       = 1;
  init.outputPin.port      = Chimera::GPIO::Port::PORTA;
  init.outputPin.alternate = Chimera::GPIO::Alternate::TIM2_CH2;
  init.outputPin.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  init.outputPin.threaded  = false;
  init.outputPin.validity  = true;
  
  init.validity = true;

  green->init( init );
  green->enableOutput();

  /*------------------------------------------------
  Initialize the Blue LED Channel
  ------------------------------------------------*/
  init.timer.countDirection = Chimera::Timer::Direction::COUNT_UP;
  init.timer.overwrite      = true;
  init.timer.peripheral     = Chimera::Timer::Peripheral::TIMER2;
  init.timer.reloadValue    = 1000;
  init.timer.prescaler      = 80;
  init.timer.validity       = true;

  init.pwm.outputChannel = Chimera::Timer::Channel::CHANNEL_4;
  init.pwm.compareMatch  = 750;
  init.pwm.polarity      = Chimera::Timer::PWM::Polarity::ACTIVE_HIGH;
  init.pwm.mode          = Chimera::Timer::PWM::Mode::EDGE_ALIGNED;
  init.pwm.validity      = true;

  init.outputPin.pin       = 3;
  init.outputPin.port      = Chimera::GPIO::Port::PORTA;
  init.outputPin.alternate = Chimera::GPIO::Alternate::TIM2_CH4;
  init.outputPin.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  init.outputPin.threaded  = false;
  init.outputPin.validity  = true;
  
  init.validity = true;

  blue->init( init );
  blue->enableOutput();

  while ( 1 )
  {
    Chimera::delayMilliseconds( 250 );
  }
}