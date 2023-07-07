/******************************************************************************
 *  File Name:
 *    test_lld_sdio.cpp
 *
 *  Description:
 *    Tests the SDIO LLD driver
 *
 *  2023 | Brandon Braun | brandonbraun653@protonmail.com
 *****************************************************************************/

/*-----------------------------------------------------------------------------
Includes
-----------------------------------------------------------------------------*/
#include <tests/harness/mbed_test_harness.hpp>

#include "CppUTest/TestHarness.h"

/*-----------------------------------------------------------------------------
Public Functions
-----------------------------------------------------------------------------*/
int main()
{
  return Thor::Testing::ExecuteMainTestThread();
}

/*-----------------------------------------------------------------------------
Test Group
-----------------------------------------------------------------------------*/
TEST_GROUP(FirstTestGroup)
{
};

TEST(FirstTestGroup, FirstTest)
{
   CHECK_EQUAL(1, 1);
}