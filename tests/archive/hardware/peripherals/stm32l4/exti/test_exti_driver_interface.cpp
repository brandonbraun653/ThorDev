/********************************************************************************
 *  File Name:
 *    test_exti_driver_interface.cpp
 *
 *  Description:
 *    Tests the STM32L4 LLD EXTI Interface
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <limits>

/* ETL Includes */
#include <etl/delegate.h>

/* Chimera Includes */
#include <Chimera/exti>
#include <Chimera/function>

/* Thor Includes */
#include <Thor/lld/common/types.hpp>
#include <Thor/lld/interface/exti/exti_detail.hpp>
#include <Thor/lld/interface/exti/exti_intf.hpp>
#include <Thor/lld/interface/exti/exti_prv_data.hpp>
#include <Thor/lld/interface/exti/exti_types.hpp>

#include <Thor/lld/stm32l4x/exti/hw_exti_prj.hpp>
#include <Thor/lld/stm32l4x/exti/hw_exti_types.hpp>

/* CppUTest Includes */
#include <CppUTest/TestHarness.h>

/*-------------------------------------------------------------------------------
Aliases
-------------------------------------------------------------------------------*/
namespace LLD = Thor::LLD::EXTI;

/*-------------------------------------------------------------------------------
Constants
-------------------------------------------------------------------------------*/
static constexpr size_t MIN_GPIO_EXTI_LINE = 0;
static constexpr size_t MAX_GPIO_EXTI_LINE = 15;

/*-------------------------------------------------------------------------------
Static Functions & Data
-------------------------------------------------------------------------------*/
static bool sISRFired = false;

/*-------------------------------------------------------------------------------
Testing Functions
-------------------------------------------------------------------------------*/
TEST_GROUP( STM32L4_LLD_EXTI_INTF ){};

/*-------------------------------------------------------------------------------
Public Project Implementation
-------------------------------------------------------------------------------*/
TEST( STM32L4_LLD_EXTI_INTF, NumInterruptLines )
{
  CHECK( LLD::numInterruptLines() == LLD::NUM_EXTI_LINES );
}


TEST( STM32L4_LLD_EXTI_INTF, OpenCloseOpenClose )
{
  CHECK( LLD::open() == Chimera::Status::OK );
  CHECK( LLD::close() == Chimera::Status::OK );

  CHECK( LLD::open() == Chimera::Status::OK );
  CHECK( LLD::open() == Chimera::Status::OK );

  CHECK( LLD::close() == Chimera::Status::OK );
  CHECK( LLD::close() == Chimera::Status::OK );
}


TEST( STM32L4_LLD_EXTI_INTF, SoftwareTriggerISR )
{
  /*-------------------------------------------------
  Initialize the test
  -------------------------------------------------*/
  LLD::open();

  /*-------------------------------------------------
  Exercise every GPIO line that can be triggered with
  a software interrupt. Edge triggering doesn't
  matter in this test.
  -------------------------------------------------*/
  Chimera::Function::vGeneric cb( []( void * ) { sISRFired = true; } );

  for ( size_t line = MIN_GPIO_EXTI_LINE; line < MAX_GPIO_EXTI_LINE; line++ )
  {
    /*-------------------------------------------------
    Attach the interrupt callback
    -------------------------------------------------*/
    sISRFired = false;
    CHECK( LLD::attach( line, Chimera::EXTI::EdgeTrigger::RISING_EDGE, cb ) == Chimera::Status::OK );

    /*-------------------------------------------------
    Verify the flag was set to true via the callback
    -------------------------------------------------*/
    CHECK( LLD::trigger( line ) );
    Chimera::delayMilliseconds( 5 );
    CHECK( sISRFired == true );

    /*-------------------------------------------------
    Detach the interrupt from firing
    -------------------------------------------------*/
    CHECK( LLD::detach( line ) == Chimera::Status::OK );

    /*-------------------------------------------------
    Verify it cannot fire again
    -------------------------------------------------*/
    sISRFired = false;
    CHECK( LLD::trigger( line ) );
    Chimera::delayMilliseconds( 5 );
    CHECK( sISRFired == false );
  }

  /*-------------------------------------------------
  Destroy the test
  -------------------------------------------------*/
  LLD::close();
}


TEST( STM32L4_LLD_EXTI_INTF, EnableDisable )
{
  /*-------------------------------------------------
  Initialize the test
  -------------------------------------------------*/
  LLD::open();

  /*-------------------------------------------------
  Exercise every GPIO line that can be triggered with
  a software interrupt. Edge triggering doesn't
  matter in this test.
  -------------------------------------------------*/
  Chimera::Function::vGeneric cb( []( void * ) { sISRFired = true; } );

  for ( size_t line = MIN_GPIO_EXTI_LINE; line < MAX_GPIO_EXTI_LINE; line++ )
  {
    /*-------------------------------------------------
    Attach the interrupt callback and disable (mask)
    the signal from firing.
    -------------------------------------------------*/
    sISRFired = false;
    CHECK( LLD::attach( line, Chimera::EXTI::EdgeTrigger::RISING_EDGE, cb ) == Chimera::Status::OK );
    CHECK( LLD::disable( line ) == Chimera::Status::OK );

    /*-------------------------------------------------
    Verify the flag was set to true via the callback
    only after the line is re-enabled.
    -------------------------------------------------*/
    CHECK( LLD::trigger( line ) );
    Chimera::delayMilliseconds( 5 );
    CHECK( sISRFired == false );

    CHECK( LLD::enable( line ) == Chimera::Status::OK );
    CHECK( LLD::trigger( line ) );
    Chimera::delayMilliseconds( 5 );
    CHECK( sISRFired == true );

    /*-------------------------------------------------
    Destroy the interrupt config
    -------------------------------------------------*/
    CHECK( LLD::detach( line ) == Chimera::Status::OK );
  }

  /*-------------------------------------------------
  Destroy the test
  -------------------------------------------------*/
  LLD::close();
}