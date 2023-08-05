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
#define TestChannel     ( Chimera::SDIO::Channel::SDIO1 )
#define TestPeriph      ( Thor::LLD::SDIO::SDIO1_PERIPH )
#define TestBlockCount  ( 5 )
#define TestBlockSize   ( 512 )

/*-----------------------------------------------------------------------------
Variables
-----------------------------------------------------------------------------*/
static uint8_t TestRXBuffer[ TestBlockCount * TestBlockSize ] = { 0 };
static uint8_t TestTXBuffer[ TestBlockCount * TestBlockSize ] = { 0 };

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
  Chimera::SDIO::HWConfig cfg;

  void setup()
  {
    /*-------------------------------------------------------------------------
    Grab a reference to the driver instance
    -------------------------------------------------------------------------*/
    sdio = Chimera::SDIO::getDriver( TestChannel );
    CHECK( sdio != nullptr );

    /*-------------------------------------------------------------------------
    Initialize the driver configuration
    -------------------------------------------------------------------------*/
    cfg.clear();
    cfg.channel    = Thor::Testing::BSP::IO::SDIO::Channel;
    cfg.clockSpeed = Thor::Testing::BSP::IO::SDIO::ClockSpeed;
    cfg.blockSize  = Thor::Testing::BSP::IO::SDIO::BlockSize;
    cfg.width      = Thor::Testing::BSP::IO::SDIO::BusWidth;
    cfg.clkPin     = Thor::Testing::BSP::IO::SDIO::clkPinInit;
    cfg.cmdPin     = Thor::Testing::BSP::IO::SDIO::cmdPinInit;
    cfg.dxPin[ 0 ] = Thor::Testing::BSP::IO::SDIO::d0PinInit;
    cfg.dxPin[ 1 ] = Thor::Testing::BSP::IO::SDIO::d1PinInit;
    cfg.dxPin[ 2 ] = Thor::Testing::BSP::IO::SDIO::d2PinInit;
    cfg.dxPin[ 3 ] = Thor::Testing::BSP::IO::SDIO::d3PinInit;
  }

  void teardown()
  {
    sdio->close();
    sdio = nullptr;
  }
};


TEST( IntegrationTests, Init )
{
  CHECK( sdio->open( cfg ) == Chimera::Status::OK );
}


TEST( IntegrationTests, Connect )
{
  CHECK( sdio->open( cfg ) == Chimera::Status::OK );
  CHECK( sdio->connect() == Chimera::Status::OK );
}

TEST( IntegrationTests, ReadBlock )
{
  CHECK( sdio->open( cfg ) == Chimera::Status::OK );
  CHECK( sdio->connect() == Chimera::Status::OK );
  CHECK( sdio->readBlock( 0, 1, TestRXBuffer, TestBlockSize ) == Chimera::Status::OK );
}