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
#define TestBlockCount  ( 5u )
#define TestBlockSize   ( 512u )
#define TestArrCount    ( TestBlockCount * ( TestBlockSize / sizeof( uint32_t ) ) )

/*-----------------------------------------------------------------------------
Variables
-----------------------------------------------------------------------------*/
static uint32_t TestRXBuffer[ TestArrCount ] = { 0 };
static uint32_t TestTXBuffer[ TestArrCount ] = { 0 };

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
    Initialize the test buffers
    -------------------------------------------------------------------------*/
    memset( TestRXBuffer, 0, sizeof( TestRXBuffer ) );
    memset( TestTXBuffer, 0, sizeof( TestTXBuffer ) );

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

TEST( IntegrationTests, WriteBlock )
{
  CHECK( sdio->open( cfg ) == Chimera::Status::OK );
  CHECK( sdio->connect() == Chimera::Status::OK );
  CHECK( sdio->writeBlock( 0, 1, TestTXBuffer, TestBlockSize ) == Chimera::Status::OK );
}

TEST( IntegrationTests, BlockDataIntegrity )
{
  /*---------------------------------------------------------------------------
  Connect to the device
  ---------------------------------------------------------------------------*/
  CHECK( sdio->open( cfg ) == Chimera::Status::OK );
  CHECK( sdio->connect() == Chimera::Status::OK );

  /*---------------------------------------------------------------------------
  Write a unique set of data to the card
  ---------------------------------------------------------------------------*/
  for ( size_t i = 0; i < ( TestBlockSize / sizeof( uint32_t ) ); i++ )
  {
    TestTXBuffer[ i ] = static_cast<uint32_t>( rand() );
  }

  CHECK( sdio->writeBlock( 0, 1, TestTXBuffer, TestBlockSize ) == Chimera::Status::OK );
  Chimera::delayMilliseconds( 150 );

  /*---------------------------------------------------------------------------
  Read the data back and check for equality
  ---------------------------------------------------------------------------*/
  CHECK( sdio->readBlock( 0, 1, TestRXBuffer, TestBlockSize ) == Chimera::Status::OK );
  CHECK( 0 == memcmp( TestTXBuffer, TestRXBuffer, TestBlockSize ) );
}
