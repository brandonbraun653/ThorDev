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
#include <Thor/lld/interface/inc/interrupt>
#include <Thor/lld/interface/inc/rcc>
#include <Thor/lld/interface/inc/sdio>
#include <tests/harness/mbed_test_harness.hpp>

#include "CppUTest/TestHarness.h"

/*-----------------------------------------------------------------------------
Static Functions
-----------------------------------------------------------------------------*/


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
namespace LLD = ::Thor::LLD::SDIO;

TEST_GROUP( DriverTests )
{
  LLD::Driver *sdio;

  void setup()
  {
    sdio = LLD::getDriver( Chimera::SDIO::Channel::SDIO1 );
    CHECK( sdio != nullptr );
    CHECK( sdio->attach( LLD::SDIO1_PERIPH ) == Chimera::Status::OK );
  }

  void teardown()
  {
    sdio->reset();
    sdio = nullptr;
  }
};


TEST( DriverTests, Reset )
{
  CHECK( sdio->reset() == Chimera::Status::OK );
}


TEST( DriverTests, ClockEnableDisable )
{
  using namespace Thor::LLD::RCC;

  /*---------------------------------------------------------------------------
  Enable the clock to get us to a starting state. There is really no way to
  guarantee the clock is disabled before the test starts, so this is the best
  we can do.
  ---------------------------------------------------------------------------*/
  sdio->clockEnable();
  uint32_t clockStatus = RCC1_PERIPH->APB2ENR;
  CHECK( ( clockStatus & APB2ENR_SDIOEN_Msk ) == APB2ENR_SDIOEN );

  /*---------------------------------------------------------------------------
  Disable the clock and verify
  ---------------------------------------------------------------------------*/
  sdio->clockDisable();
  clockStatus = RCC1_PERIPH->APB2ENR;
  CHECK( ( clockStatus & APB2ENR_SDIOEN_Msk ) == 0x0 );

  /*---------------------------------------------------------------------------
  Re-enable the clock and verify
  ---------------------------------------------------------------------------*/
  sdio->clockEnable();
  clockStatus = RCC1_PERIPH->APB2ENR;
  CHECK( ( clockStatus & APB2ENR_SDIOEN_Msk ) == APB2ENR_SDIOEN );
}


TEST( DriverTests, EnterExitCriticalSection )
{
  /*---------------------------------------------------------------------------
  Ensure we've started in a known state
  ---------------------------------------------------------------------------*/
  sdio->clockEnable();
  sdio->reset();
  CHECK( Thor::LLD::INT::getActive( LLD::Resource::IRQSignals[ LLD::SDIO1_RESOURCE_INDEX ] ) == 0 );

  /*---------------------------------------------------------------------------
  Enable the interrupt and verify
  ---------------------------------------------------------------------------*/
  sdio->exitCriticalSection();
  CHECK( true == Thor::LLD::INT::isEnabledIRQ( LLD::Resource::IRQSignals[ LLD::SDIO1_RESOURCE_INDEX ] ) );

  /*---------------------------------------------------------------------------
  Disable the interrupt and verify
  ---------------------------------------------------------------------------*/
  sdio->enterCriticalSection();
  CHECK( false == Thor::LLD::INT::isEnabledIRQ( LLD::Resource::IRQSignals[ LLD::SDIO1_RESOURCE_INDEX ] ) );
}
