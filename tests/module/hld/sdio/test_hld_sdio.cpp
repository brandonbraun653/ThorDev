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
Initialization of the HLD Driver
-----------------------------------------------------------------------------*/
TEST_GROUP( SDIOHLD_Open )
{
  Thor::LLD::SDIO::Driver_rPtr pLLDriver;
  Chimera::SDIO::Driver_rPtr driver;
  Chimera::SDIO::HWConfig cfg;

  void setup()
  {
    /*-------------------------------------------------------------------------
    Mock out the LLD interface
    -------------------------------------------------------------------------*/
    expect::Thor$::LLD$::SDIO$::initialize( 1, Chimera::Status::OK );
    expect::Thor$::LLD$::SDIO$::isSupported( 1, TestChannel, true );
    expect::Thor$::LLD$::SDIO$::Driver$::Driver$ctor();

    /*-------------------------------------------------------------------------
    Reset test memory
    -------------------------------------------------------------------------*/
    driver = nullptr;
    pLLDriver = new Thor::LLD::SDIO::Driver();
    cfg.clear();

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
    expect::Thor$::LLD$::SDIO$::Driver$::Driver$dtor( pLLDriver );

    delete pLLDriver;

    mock().checkExpectations();
    mock().clear();
  }
};


TEST( SDIOHLD_Open, InvalidResourceIndex )
{
  cfg.channel = TestChannel;
  expect::Thor$::LLD$::SDIO$::getResourceIndex( TestChannel, ::Thor::LLD::INVALID_RESOURCE_INDEX );

  CHECK( driver->open( cfg ) == Chimera::Status::INVAL_FUNC_PARAM );

  mock().checkExpectations();
}


TEST( SDIOHLD_Open, BindAndInitDrivers )
{
  cfg.channel = TestChannel;
  expect::Thor$::LLD$::SDIO$::getResourceIndex( TestChannel, ::Thor::LLD::SDIO::SDIO1_RESOURCE_INDEX );
  expect::Thor$::LLD$::SDIO$::getDriver( TestChannel, pLLDriver );
  expect::Thor$::LLD$::SDIO$::Driver$::init( pLLDriver, Chimera::Status::OK );

  CHECK( driver->open( cfg ) == Chimera::Status::OK );

  mock().checkExpectations();
}


TEST( SDIOHLD_Open, ReturnStatusOfLLDInit )
{
  cfg.channel = TestChannel;
  expect::Thor$::LLD$::SDIO$::getResourceIndex( TestChannel, ::Thor::LLD::SDIO::SDIO1_RESOURCE_INDEX );
  expect::Thor$::LLD$::SDIO$::getDriver( TestChannel, pLLDriver );
  expect::Thor$::LLD$::SDIO$::Driver$::init( pLLDriver, Chimera::Status::FAIL );

  CHECK( driver->open( cfg ) == Chimera::Status::FAIL );

  mock().checkExpectations();
}


/*-----------------------------------------------------------------------------
Test standard usage scenarios of the driver that depend on it being initialized
-----------------------------------------------------------------------------*/
TEST_GROUP( SDIOHLD_Usage )
{
  Thor::LLD::SDIO::Driver_rPtr pLLDriver;
  Chimera::SDIO::Driver_rPtr driver;
  Chimera::SDIO::HWConfig cfg;

  void setup()
  {
    /*-------------------------------------------------------------------------
    Mock out the LLD interface
    -------------------------------------------------------------------------*/
    expect::Thor$::LLD$::SDIO$::initialize( 1, Chimera::Status::OK );
    expect::Thor$::LLD$::SDIO$::isSupported( 1, TestChannel, true );
    expect::Thor$::LLD$::SDIO$::Driver$::Driver$ctor();

    /*-------------------------------------------------------------------------
    Reset test memory
    -------------------------------------------------------------------------*/
    driver = nullptr;
    pLLDriver = new Thor::LLD::SDIO::Driver();
    cfg.clear();

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

    /*-------------------------------------------------------------------------
    Setup the driver for testing
    -------------------------------------------------------------------------*/
    cfg.channel = TestChannel;
    expect::Thor$::LLD$::SDIO$::getResourceIndex( TestChannel, ::Thor::LLD::SDIO::SDIO1_RESOURCE_INDEX );
    expect::Thor$::LLD$::SDIO$::getDriver( TestChannel, pLLDriver );
    expect::Thor$::LLD$::SDIO$::Driver$::init( pLLDriver, Chimera::Status::OK );

    CHECK( driver->open( cfg ) == Chimera::Status::OK );
    mock().checkExpectations();
  }

  void teardown()
  {
    expect::Thor$::LLD$::SDIO$::Driver$::Driver$dtor( pLLDriver );

    delete pLLDriver;

    mock().checkExpectations();
    mock().clear();
  }
};


TEST( SDIOHLD_Usage, connect_PowerTransitionFailure )
{
  /*---------------------------------------------------------------------------
  Set expectations
  ---------------------------------------------------------------------------*/
  expect::Thor$::LLD$::SDIO$::Driver$::busClockDisable( 1, pLLDriver );
  expect::Thor$::LLD$::SDIO$::Driver$::setPowerStateOn( 1, pLLDriver, Chimera::Status::CRC_ERROR );

  /*-------------------------------------------------------------------------
  Run the test
  -------------------------------------------------------------------------*/
  CHECK( driver->connect() == Chimera::Status::FAIL );
  mock().checkExpectations();
}
