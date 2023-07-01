/******************************************************************************
 *  File Name:
 *    test_lld_intf_sdio.cpp
 *
 *  Description:
 *    Tests the SDIO LLD Interface driver
 *
 *  2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <Thor/lld/interface/inc/sdio>

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

int main(int ac, char** av)
{
   return CommandLineTestRunner::RunAllTests(ac, av);
}

/*-----------------------------------------------------------------------------
Tests
-----------------------------------------------------------------------------*/

TEST_GROUP(LLD_Interface_SDIO)
{
};

TEST(LLD_Interface_SDIO, ResourceIndexMapping)
{
  using namespace Thor::LLD::SDIO;
  
  CHECK_EQUAL( 0, getResourceIndex( Thor::LLD::SDIO::SDIO1_BASE_ADDR ) );
}