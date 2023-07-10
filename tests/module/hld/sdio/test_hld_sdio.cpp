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

namespace Thor::LLD::SDIO
{
}

/*-----------------------------------------------------------------------------
Test High Level User Interface
-----------------------------------------------------------------------------*/
TEST_GROUP( UserInterface )
{
  void setup()
  {
  }

};

TEST( UserInterface, Initialize )
{
  Chimera::Status_t result = Chimera::SDIO::initialize();

  CHECK_EQUAL( Chimera::Status::NOT_SUPPORTED, result );
}
