/********************************************************************************
 *  File Name:
 *    test_lfs_integration.cpp
 *
 *  Description:
 *    HIL integration testing with Little FS. This requires several pieces of
 *    hardware:
 *      1. Development board that exposes an SPI port.
 *      2. A flash driver of some kind
 *      3. NOR flash memory chip with an SPI interface.
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/gpio>
#include <Chimera/spi>
#include <Chimera/thread>

/* Aurora Includes */
#include <Aurora/memory>

/* Little FS Includes */
#include "lfs.h"

/* Memory Driver Includes */
#include <Adesto/at25/at25_driver.hpp>

/* Test Framework Includes */

static constexpr Chimera::SPI::Channel spiChannel = Chimera::SPI::Channel::SPI1;

static void test_thread( void *arg );

int main()
{
  using namespace Chimera::Threading;
  ChimeraInit();

  Thread testing;
  testing.initialize( test_thread, nullptr, Priority::LEVEL_3, 5000, "test" );
  testing.start();

  startScheduler();
  return 0;
}

static void test_thread( void *arg )
{
  /*-------------------------------------------------
  Initialize the SPI driver
  -------------------------------------------------*/
  Chimera::SPI::DriverConfig cfg;
  cfg.clear();
  cfg.validity = true;

  cfg.HWInit.bitOrder    = Chimera::SPI::BitOrder::MSB_FIRST;
  cfg.HWInit.clockFreq   = 8000000;
  cfg.HWInit.clockMode   = Chimera::SPI::ClockMode::MODE0;
  cfg.HWInit.dataSize    = Chimera::SPI::DataSize::SZ_8BIT;
  cfg.HWInit.hwChannel   = spiChannel;
  cfg.HWInit.txfrMode    = Chimera::SPI::TransferMode::INTERRUPT;
  cfg.HWInit.controlMode = Chimera::SPI::ControlMode::MASTER;
  cfg.HWInit.csMode      = Chimera::SPI::CSMode::MANUAL;
  cfg.HWInit.validity    = true;

  // Chip Select
  cfg.externalCS       = false;
  cfg.CSInit.alternate = Chimera::GPIO::Alternate::NONE;
  cfg.CSInit.drive     = Chimera::GPIO::Drive::OUTPUT_PUSH_PULL;
  cfg.CSInit.pin       = 15;
  cfg.CSInit.port      = Chimera::GPIO::Port::PORTC;
  cfg.CSInit.pull      = Chimera::GPIO::Pull::PULL_UP;
  cfg.CSInit.state     = Chimera::GPIO::State::HIGH;
  cfg.CSInit.threaded  = false;
  cfg.CSInit.validity  = true;

  // SCK
  cfg.SCKInit.alternate = Chimera::GPIO::Alternate::SPI1_SCK;
  cfg.SCKInit.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  cfg.SCKInit.pin       = 5;
  cfg.SCKInit.port      = Chimera::GPIO::Port::PORTA;
  cfg.SCKInit.threaded  = false;
  cfg.SCKInit.validity  = true;

  // MISO
  cfg.MISOInit.alternate = Chimera::GPIO::Alternate::SPI1_MISO;
  cfg.MISOInit.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  cfg.MISOInit.pin       = 6;
  cfg.MISOInit.port      = Chimera::GPIO::Port::PORTA;
  cfg.MISOInit.threaded  = false;
  cfg.MISOInit.validity  = true;

  // MOSI
  cfg.MOSIInit.alternate = Chimera::GPIO::Alternate::SPI1_MOSI;
  cfg.MOSIInit.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
  cfg.MOSIInit.pin       = 7;
  cfg.MOSIInit.port      = Chimera::GPIO::Port::PORTA;
  cfg.MOSIInit.threaded  = false;
  cfg.MOSIInit.validity  = true;

  auto spi = Chimera::SPI::getDriver( spiChannel );
  spi->init( cfg );

  /*-------------------------------------------------
  Initialize the Adesto Driver
  -------------------------------------------------*/
  Adesto::AT25::DeviceInfo info;
  memset( &info, 0, sizeof( info ) );

  auto mem = Adesto::AT25::Driver();
  mem.configure( spiChannel );
  mem.readDeviceInfo( info );

  /*-------------------------------------------------
  Break into the debugger
  -------------------------------------------------*/
  while ( 1 )
  {
    Chimera::insert_debug_breakpoint();
    info.density = Adesto::DENSITY_32MBIT;
  }
}