/******************************************************************************
 *  File Name:
 *    test_hld_sdio.cpp
 *
 *  Description:
 *    Tests the SDIO HLD driver
 *
 *  2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Chimera/sdio>
#include <mock/Thor/lld/interface/sdio/sdio_driver_expect.hpp>
#include <mock/Thor/lld/interface/sdio/sdio_intf_expect.hpp>

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTestExt/MockSupport.h"

/*-----------------------------------------------------------------------------
Static Data
-----------------------------------------------------------------------------*/
static constexpr Chimera::SDIO::Channel TestChannel = Chimera::SDIO::Channel::SDIO1;

/*-----------------------------------------------------------------------------
Public Functions
-----------------------------------------------------------------------------*/
int main( int ac, char **av )
{
  return CommandLineTestRunner::RunAllTests( ac, av );
}

/*-----------------------------------------------------------------------------
Test Chimera User Interface Implementation
-----------------------------------------------------------------------------*/
TEST_GROUP( ChimeraInterface )
{
  void teardown()
  {
    mock().clear();
  }
};

TEST( ChimeraInterface, InitializeOnceIsOK )
{
  expect::Thor$::LLD$::SDIO$::initialize( 1, Chimera::Status::OK );

  CHECK( Chimera::SDIO::initialize() == Chimera::Status::OK );
  CHECK( Chimera::SDIO::initialize() == Chimera::Status::OK );

  mock().checkExpectations();
  Chimera::SDIO::reset();
}


TEST( ChimeraInterface, ResetAllowsReInit )
{
  expect::Thor$::LLD$::SDIO$::initialize( 2, Chimera::Status::OK );

  CHECK( Chimera::SDIO::reset() == Chimera::Status::OK );
  CHECK( Chimera::SDIO::initialize() == Chimera::Status::OK );  /* First call normal */
  CHECK( Chimera::SDIO::initialize() == Chimera::Status::OK );  /* Second call is no-op */
  CHECK( Chimera::SDIO::reset() == Chimera::Status::OK );
  CHECK( Chimera::SDIO::initialize() == Chimera::Status::OK );  /* Third call re-initializes */
  CHECK( Chimera::SDIO::reset() == Chimera::Status::OK );

  mock().checkExpectations();
}


TEST( ChimeraInterface, CannotGetDriverUntilInitCalled )
{
  expect::Thor$::LLD$::SDIO$::initialize( 1, Chimera::Status::OK );
  expect::Thor$::LLD$::SDIO$::isSupported( 1, TestChannel, true );

  Chimera::SDIO::reset();
  CHECK( Chimera::SDIO::getDriver( TestChannel ) == nullptr );
  Chimera::SDIO::initialize();
  CHECK( Chimera::SDIO::getDriver( TestChannel ) != nullptr );

  mock().checkExpectations();
}


TEST( ChimeraInterface, UnsupportedChannelsReturnNullDriver )
{
  expect::Thor$::LLD$::SDIO$::initialize( 1, Chimera::Status::OK );
  expect::Thor$::LLD$::SDIO$::isSupported( 1, TestChannel, false );

  Chimera::SDIO::reset();
  Chimera::SDIO::initialize();
  CHECK( Chimera::SDIO::getDriver( TestChannel ) == nullptr );

  mock().checkExpectations();
}

/*-----------------------------------------------------------------------------
Test HLD Driver Implementation
-----------------------------------------------------------------------------*/
TEST_GROUP( SDIOHLD )
{
  Chimera::SDIO::Driver_rPtr driver;
  Chimera::SDIO::HWConfig cfg;

  void setup()
  {
    /*-------------------------------------------------------------------------
    Reset test memory
    -------------------------------------------------------------------------*/
    driver = nullptr;
    cfg.clear();

    /*-------------------------------------------------------------------------
    Mock out the LLD interface
    -------------------------------------------------------------------------*/
    expect::Thor$::LLD$::SDIO$::initialize( 1, Chimera::Status::OK );
    expect::Thor$::LLD$::SDIO$::isSupported( 1, TestChannel, true );

    /*-------------------------------------------------------------------------
    Initialize the driver to prepare for testing
    -------------------------------------------------------------------------*/
    Chimera::SDIO::reset();
    Chimera::SDIO::initialize();
    driver = Chimera::SDIO::getDriver( TestChannel );

    /*-------------------------------------------------------------------------
    Validate setup conditions
    -------------------------------------------------------------------------*/
    CHECK( driver != nullptr );
    mock().checkExpectations();
  }

  void teardown()
  {
    mock().clear();
  }
};


TEST( SDIOHLD, open_InvalidResourceIndex )
{
  cfg.channel = TestChannel;
  expect::Thor$::LLD$::SDIO$::getResourceIndex( TestChannel, ::Thor::LLD::INVALID_RESOURCE_INDEX );

  CHECK( driver->open( cfg ) == Chimera::Status::INVAL_FUNC_PARAM );
}
