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

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTestExt/MockSupport.h"

/*-----------------------------------------------------------------------------
Static Data
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
Public Functions
-----------------------------------------------------------------------------*/
int main( int ac, char **av )
{
  return CommandLineTestRunner::RunAllTests( ac, av );
}

/*-----------------------------------------------------------------------------
Test High Level User Interface
-----------------------------------------------------------------------------*/
TEST_GROUP( UserInterface )
{
  Chimera::SDIO::Driver_rPtr sdio;

  void setup()
  {
    sdio = Chimera::SDIO::getDriver( Chimera::SDIO::Channel::SDIO1 );
    CHECK( sdio != nullptr );
  }

};

TEST( UserInterface, Initialize )
{
  using namespace Chimera::SDIO;
  HWConfig cfg;

  expect::Thor$::LLD$::SDIO$::Driver$::init( 1, nullptr, Chimera::Status::OK );

  CHECK( sdio->open( cfg ) == Chimera::Status::OK );

  mock().checkExpectations();
}
