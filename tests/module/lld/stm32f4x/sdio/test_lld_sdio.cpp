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
Constants
-----------------------------------------------------------------------------*/
#define TestChannel ( Chimera::SDIO::Channel::SDIO1 )
#define TestPeriph ( Thor::LLD::SDIO::SDIO1_PERIPH )

/*-----------------------------------------------------------------------------
Public Functions
-----------------------------------------------------------------------------*/
int main()
{
  return Thor::Testing::ExecuteMainTestThread();
}

/*-----------------------------------------------------------------------------
Test Group: Isolated Functional Behavior
-----------------------------------------------------------------------------*/
namespace LLD = ::Thor::LLD::SDIO;

TEST_GROUP( IsolatedFuncTests )
{
  LLD::Driver *sdio;

  void setup()
  {
    sdio = LLD::getDriver( TestChannel );
    CHECK( sdio != nullptr );
    CHECK( sdio->attach( TestPeriph ) == Chimera::Status::OK );
  }

  void teardown()
  {
    sdio->reset();
    sdio = nullptr;
  }
};


TEST( IsolatedFuncTests, Reset )
{
  CHECK( sdio->reset() == Chimera::Status::OK );
}


TEST( IsolatedFuncTests, ClockEnableDisable )
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


TEST( IsolatedFuncTests, ClockAllowsRegisterAccess )
{
  sdio->clockEnable();

  LLD::PWRCTRL::set( TestPeriph, 0x3 << LLD::POWER_PWRCTRL_Pos );
  CHECK( LLD::PWRCTRL::get( TestPeriph ) == ( 0x3 << LLD::POWER_PWRCTRL_Pos ) );

  LLD::PWRCTRL::clear( TestPeriph, 0x3 << LLD::POWER_PWRCTRL_Pos );
  CHECK( LLD::PWRCTRL::get( TestPeriph ) == 0x0 );

  sdio->clockDisable();
}


TEST( IsolatedFuncTests, EnterExitCriticalSection )
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


TEST( IsolatedFuncTests, ClockBusOutputEnableDisable)
{
  /*---------------------------------------------------------------------------
  Ensure we've started in a known state
  ---------------------------------------------------------------------------*/
  sdio->clockEnable();
  sdio->reset();
  CHECK( ( TestPeriph->CLKCR & LLD::CLKCR_CLKEN_Msk ) == 0x0 );

  /*---------------------------------------------------------------------------
  Enable the clock and verify
  ---------------------------------------------------------------------------*/
  sdio->busClockEnable();
  CHECK( ( TestPeriph->CLKCR & LLD::CLKCR_CLKEN_Msk ) == LLD::CLKCR_CLKEN );

  /*---------------------------------------------------------------------------
  Disable the clock and verify
  ---------------------------------------------------------------------------*/
  sdio->busClockDisable();
  CHECK( ( TestPeriph->CLKCR & LLD::CLKCR_CLKEN_Msk ) == 0x0 );
}

/*-----------------------------------------------------------------------------
Test Group: System Configuration Tests
-----------------------------------------------------------------------------*/
TEST_GROUP( SysConfigTests )
{
  LLD::Driver *sdio;

  void setup()
  {
    sdio = LLD::getDriver( TestChannel );
    CHECK( sdio != nullptr );
    CHECK( sdio->attach( TestPeriph ) == Chimera::Status::OK );
    sdio->clockEnable();
    sdio->reset();
  }

  void teardown()
  {
    sdio->reset();
    sdio->clockDisable();
    sdio = nullptr;
  }
};


TEST( SysConfigTests, SDInit )
{
  /*---------------------------------------------------------------------------
  Ensure we've started in a known state
  ---------------------------------------------------------------------------*/
  CHECK( TestPeriph->CLKCR == 0x0 );

  /*---------------------------------------------------------------------------
  Invoke FUT
  ---------------------------------------------------------------------------*/
  CHECK( sdio->init() == Chimera::Status::OK );

  /*---------------------------------------------------------------------------
  Verify the results
  ---------------------------------------------------------------------------*/
  const uint32_t clk_cfg = TestPeriph->CLKCR;
  CHECK( ( clk_cfg & LLD::CLKCR_HWFC_EN_Msk ) == 0x0 ); // Hardware flow control disabled
  CHECK( ( clk_cfg & LLD::CLKCR_WIDBUS_Msk ) == 0x0 );  // 1-bit bus width (for now)
  CHECK( ( clk_cfg & LLD::CLKCR_BYPASS_Msk ) == 0x0 );  // No clock bypass
  CHECK( ( clk_cfg & LLD::CLKCR_PWRSAV_Msk ) == 0x0 );  // Power saving disabled
  CHECK( ( clk_cfg & LLD::CLKCR_CLKEN_Msk ) == 0x0 );   // Clock disabled (for now)
  CHECK( sdio->getBusFrequency() <= 400000 );           // Verify RM0390 29.8.2 SDIO_CLKCR Note 1
}

/*-----------------------------------------------------------------------------
Test Group: Integration Tests (requires SD card connection)
-----------------------------------------------------------------------------*/
TEST_GROUP( IntegrationTests )
{
  LLD::Driver *sdio;

  void setup()
  {
    sdio = LLD::getDriver( TestChannel );
    CHECK( sdio != nullptr );
    CHECK( sdio->attach( TestPeriph ) == Chimera::Status::OK );
    sdio->clockEnable();
    sdio->reset();
    sdio->init();
  }

  void teardown()
  {
    sdio->reset();
    sdio->clockDisable();
    sdio = nullptr;
  }
};
