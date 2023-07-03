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
#include <Chimera/sdio>
#include <Thor/lld/interface/inc/sdio>
#include <mock/Thor/lld/common/cortex-m4/interrupts_expect.hpp>

#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

/*-----------------------------------------------------------------------------
Application Entry
-----------------------------------------------------------------------------*/
int main( int ac, char **av )
{
  return CommandLineTestRunner::RunAllTests( ac, av );
}

using namespace Thor::LLD;

/*-----------------------------------------------------------------------------
Test Public Interface
-----------------------------------------------------------------------------*/
TEST_GROUP( LLD_Interface_SDIO ){};

TEST( LLD_Interface_SDIO, ChannelSupport )
{
  /*---------------------------------------------------------------------------
  Allocate Expected Results
  ---------------------------------------------------------------------------*/
  const std::array<bool, EnumValue( Chimera::SDIO::Channel::NUM_OPTIONS )> SupportedChannels = {
#if defined( STM32_SDIO1_PERIPH_AVAILABLE )
    true,
#else
    false,
#endif
#if defined( STM32_SDIO2_PERIPH_AVAILABLE )
    true,
#else
    false,
#endif
  };

  for( size_t idx = 0; idx < SupportedChannels.size(); idx++ )
  {
    CHECK_EQUAL( SupportedChannels[ idx ], SDIO::isSupported( static_cast<Chimera::SDIO::Channel>( idx ) ) );
  }

  /*---------------------------------------------------------------------------
  Ensure abnormal values are handled
  ---------------------------------------------------------------------------*/
  CHECK_EQUAL( false, SDIO::isSupported( Chimera::SDIO::Channel::NUM_OPTIONS ) );
  CHECK_EQUAL( false, SDIO::isSupported( Chimera::SDIO::Channel::UNKNOWN ) );
}


TEST( LLD_Interface_SDIO, ResourceIndexMapping )
{
  CHECK_EQUAL( SDIO::SDIO1_RESOURCE_INDEX, SDIO::getResourceIndex( reinterpret_cast<std::uintptr_t>( SDIO::SDIO1_PERIPH ) ) );
  CHECK_EQUAL( INVALID_RESOURCE_INDEX, SDIO::getResourceIndex( 0xFFFFFFFF ) );
}


TEST( LLD_Interface_SDIO, MapChannelAddressToEnum )
{
#if defined( STM32_SDIO1_PERIPH_AVAILABLE )
  CHECK( Chimera::SDIO::Channel::SDIO1 == SDIO::getChannel( reinterpret_cast<std::uintptr_t>( SDIO::SDIO1_PERIPH ) ) );
#endif
#if defined( STM32_SDIO2_PERIPH_AVAILABLE )
  CHECK( Chimera::SDIO::Channel::SDIO2 == SDIO::getChannel( reinterpret_cast<std::uintptr_t>( SDIO::SDIO2_PERIPH ) ) );
#endif

  CHECK( Chimera::SDIO::Channel::UNKNOWN == SDIO::getChannel( 0xFFFFFFFF ) );
}


TEST( LLD_Interface_SDIO, DriverAttachment )
{
  SDIO::Driver driverList[ SDIO::NUM_SDIO_PERIPHS ];

  /*---------------------------------------------------------------------------
  Bad arguments
  ---------------------------------------------------------------------------*/
  CHECK_EQUAL( false, SDIO::attachDriverInstances( nullptr, SDIO::NUM_SDIO_PERIPHS ) );
  CHECK_EQUAL( false, SDIO::attachDriverInstances( driverList, 0 ) );

  /*---------------------------------------------------------------------------
  Check that the driver is reported as attached
  ---------------------------------------------------------------------------*/
  CHECK_EQUAL( true, SDIO::attachDriverInstances( driverList, SDIO::NUM_SDIO_PERIPHS ) );
}

/*-----------------------------------------------------------------------------
Test Common Driver
-----------------------------------------------------------------------------*/
