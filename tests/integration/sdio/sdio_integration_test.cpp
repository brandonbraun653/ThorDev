/******************************************************************************
 *  File Name:
 *    test_lld_sdio.cpp
 *
 *  Description:
 *    Integration testing the Thor SDIO driver
 *
 *  2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/sdio>
#include <Thor/lld/interface/inc/sdio>
#include <tests/harness/mbed_test_harness.hpp>
#include <tests/harness/mbed_test_harness_bsp.hpp>

#include "CppUTest/TestHarness.h"

/*-----------------------------------------------------------------------------
Constants
-----------------------------------------------------------------------------*/
#define TestChannel ( Chimera::SDIO::Channel::SDIO1 )
#define TestPeriph  ( Thor::LLD::SDIO::SDIO1_PERIPH )

/*-----------------------------------------------------------------------------
Public Functions
-----------------------------------------------------------------------------*/
int main()
{
  return Thor::Testing::ExecuteMainTestThread();
}


/*-----------------------------------------------------------------------------
Test Group: Integration Tests (requires SD card connection)
-----------------------------------------------------------------------------*/
TEST_GROUP( IntegrationTests )
{
  Chimera::SDIO::Driver_rPtr sdio;

  void setup()
  {
    sdio = Chimera::SDIO::getDriver( TestChannel );
    CHECK( sdio != nullptr );
  }

  void teardown()
  {
    sdio = nullptr;
  }
};


TEST( IntegrationTests, Init )
{
  Chimera::SDIO::HWConfig cfg;

  cfg.clear();
  cfg.channel    = Thor::Testing::BSP::IO::SDIO::Channel;
  cfg.clockSpeed = Thor::Testing::BSP::IO::SDIO::ClockSpeed;
  cfg.width      = Thor::Testing::BSP::IO::SDIO::BusWidth;
  cfg.clkPin     = Thor::Testing::BSP::IO::SDIO::clkPinInit;
  cfg.cmdPin     = Thor::Testing::BSP::IO::SDIO::cmdPinInit;
  cfg.dxPin[ 0 ] = Thor::Testing::BSP::IO::SDIO::d0PinInit;
  cfg.dxPin[ 1 ] = Thor::Testing::BSP::IO::SDIO::d1PinInit;
  cfg.dxPin[ 2 ] = Thor::Testing::BSP::IO::SDIO::d2PinInit;
  cfg.dxPin[ 3 ] = Thor::Testing::BSP::IO::SDIO::d3PinInit;

  CHECK( sdio->open( cfg ) == Chimera::Status::OK );
}
