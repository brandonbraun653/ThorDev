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
#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

int main(int ac, char** av)
{
   return CommandLineTestRunner::RunAllTests(ac, av);
}

TEST_GROUP(FirstTestGroup)
{
};

TEST(FirstTestGroup, FirstTest)
{
   CHECK_EQUAL(1, 1);
}