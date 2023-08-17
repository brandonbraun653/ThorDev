/********************************************************************************
 *  File Name:
 *    test_driver_interface.cpp
 *
 *  Description:
 *    Tests the STM32L4 LLD CAN Interface
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <limits>

/* Chimera Includes */
#include <Chimera/can>

/* Thor Includes */
#include <Thor/lld/common/types.hpp>
#include <Thor/lld/interface/can/can_detail.hpp>
#include <Thor/lld/interface/can/can_intf.hpp>
#include <Thor/lld/interface/can/can_prv_data.hpp>
#include <Thor/lld/interface/can/can_types.hpp>

#include <Thor/lld/stm32l4x/can/hw_can_prj.hpp>
#include <Thor/lld/stm32l4x/can/hw_can_prv_driver.hpp>
#include <Thor/lld/stm32l4x/can/hw_can_types.hpp>

/* CppUTest Includes */
#include <CppUTest/TestHarness.h>

/*-------------------------------------------------------------------------------
Aliases
-------------------------------------------------------------------------------*/
namespace LLD = Thor::LLD::CAN;

/*-------------------------------------------------------------------------------
Testing Functions
-------------------------------------------------------------------------------*/
TEST_GROUP( STM32L4_LLD_CAN_INTF ){};


/*-------------------------------------------------------------------------------
Public Project Implementation
-------------------------------------------------------------------------------*/
TEST( STM32L4_LLD_CAN_INTF, Project_Initialize )
{
  /*-------------------------------------------------
  Verify
  -------------------------------------------------*/
  CHECK( LLD::initialize() == Chimera::Status::OK );
  CHECK( LLD::initialize() == Chimera::Status::OK );
}


TEST( STM32L4_LLD_CAN_INTF, Project_GetDriver )
{
  /*-------------------------------------------------
  Call FUT
  -------------------------------------------------*/
  LLD::Driver_rPtr can1 = LLD::getDriver( Chimera::CAN::Channel::CAN0 );
  LLD::Driver_rPtr can2 = LLD::getDriver( Chimera::CAN::Channel::CAN1 );
  LLD::Driver_rPtr can3 = LLD::getDriver( Chimera::CAN::Channel::CAN2 );
  LLD::Driver_rPtr canx = LLD::getDriver( Chimera::CAN::Channel::NUM_OPTIONS );

  /*-------------------------------------------------
  Verify
  -------------------------------------------------*/
  CHECK( can1 != nullptr );
  CHECK( can2 == nullptr );
  CHECK( can3 == nullptr );
  CHECK( canx == nullptr );
}


/*-------------------------------------------------------------------------------
Interface Layer Tests
-------------------------------------------------------------------------------*/
TEST( STM32L4_LLD_CAN_INTF, Intf_IsSupported )
{
  CHECK( LLD::isSupported( Chimera::CAN::Channel::CAN0 ) == true );
  CHECK( LLD::isSupported( Chimera::CAN::Channel::CAN1 ) == false );
  CHECK( LLD::isSupported( Chimera::CAN::Channel::CAN2 ) == false );
  CHECK( LLD::isSupported( Chimera::CAN::Channel::NUM_OPTIONS ) == false );
}


TEST( STM32L4_LLD_CAN_INTF, Intf_ResourceIndex )
{
  // Index by peripheral address
  CHECK( Thor::LLD::INVALID_RESOURCE_INDEX == LLD::getResourceIndex( reinterpret_cast<std::uintptr_t>( nullptr ) ) );
  CHECK( LLD::CAN1_RESOURCE_INDEX == LLD::getResourceIndex( reinterpret_cast<std::uintptr_t>( LLD::CAN1_PERIPH ) ) );

  // Index by channel
  CHECK( LLD::CAN1_RESOURCE_INDEX == LLD::getResourceIndex( Chimera::CAN::Channel::CAN0 ) );
  CHECK( Thor::LLD::INVALID_RESOURCE_INDEX == LLD::getResourceIndex( Chimera::CAN::Channel::CAN1 ) );
  CHECK( Thor::LLD::INVALID_RESOURCE_INDEX == LLD::getResourceIndex( Chimera::CAN::Channel::CAN2 ) );
  CHECK( Thor::LLD::INVALID_RESOURCE_INDEX == LLD::getResourceIndex( Chimera::CAN::Channel::NUM_OPTIONS ) );
}


TEST( STM32L4_LLD_CAN_INTF, Intf_GetChannel )
{
  CHECK( Chimera::CAN::Channel::UNKNOWN == LLD::getChannel( reinterpret_cast<std::uintptr_t>( nullptr ) ) );
  CHECK( Chimera::CAN::Channel::CAN0 == LLD::getChannel( reinterpret_cast<std::uintptr_t>( LLD::CAN1_PERIPH ) ) );
}


TEST( STM32L4_LLD_CAN_INTF, Intf_AttachDriver)
{
  std::array<LLD::Driver, LLD::NUM_CAN_PERIPHS> fakeDrivers;

  // Bad inputs
  CHECK( false == LLD::attachDriverInstances( nullptr, LLD::NUM_CAN_PERIPHS ) );
  CHECK( false == LLD::attachDriverInstances( fakeDrivers.data(), 0 ) );
  CHECK( false == LLD::attachDriverInstances( fakeDrivers.data(), std::numeric_limits<size_t>::max() ) );

  // Good inputs
  CHECK( true == LLD::attachDriverInstances( fakeDrivers.data(), LLD::NUM_CAN_PERIPHS ) );
}