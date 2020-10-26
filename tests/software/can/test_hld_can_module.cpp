/********************************************************************************
 *  File Name:
 *    test_module_module.cpp
 *
 *  Description:
 *    Primary test driver for Thor HLD CAN
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/common>

/* Thor Includes */
#include <Thor/can>
#include <Thor/lld/interface/can/can_intf.hpp>
#include <Thor/lld/interface/can/can_types.hpp>
#include <Thor/lld/interface/can/can_detail.hpp>

/* Testing Includes */
#include <gtest/gtest.h>
#include "can_test_fixture.hpp"

namespace LLD = ::Thor::LLD::CAN;

namespace Thor::CAN
{
  using ::testing::Exactly;
  using ::testing::Return;

  /*-------------------------------------------------------------------------------
  initialize()
    - On the first call will initialize the LLD
    - Subsequent calls will return immediately as everything is already init'd
    - Bad initialization of the LLD will propagate upwards
  -------------------------------------------------------------------------------*/
  TEST_F( CANModuleTest, Initialize )
  {
    /*-------------------------------------------------
    Setup
    -------------------------------------------------*/
    EXPECT_CALL( MockObj(), initialize() ).Times( Exactly( 1 ) ).WillRepeatedly( Return( Chimera::Status::OK ) );

    /*-------------------------------------------------
    Verify
    -------------------------------------------------*/
    EXPECT_EQ( Chimera::Status::OK, initialize() );    // First time sets up the driver
    EXPECT_EQ( Chimera::Status::OK, initialize() );    // Second time is already initialized
  }

  TEST_F( CANModuleTest, Initialize_LLD_Fail )
  {
    /*-------------------------------------------------
    Setup
    -------------------------------------------------*/
    EXPECT_CALL( MockObj(), initialize() ).Times( Exactly( 1 ) ).WillRepeatedly( Return( Chimera::Status::FAILED_INIT ) );

    /*-------------------------------------------------
    Verify
    -------------------------------------------------*/
    EXPECT_EQ( Chimera::Status::FAILED_INIT, initialize() );    // First time sets up the driver
    EXPECT_EQ( Chimera::Status::OK, initialize() );             // Second time is already initialized
  }


  /*-------------------------------------------------------------------------------
  reset()
    - Should unlock the initialization sequence again
  -------------------------------------------------------------------------------*/
  TEST_F( CANModuleTest, Reset )
  {
    /*-------------------------------------------------
    Setup
    -------------------------------------------------*/
    EXPECT_CALL( MockObj(), initialize() ).Times( Exactly( 2 ) ).WillRepeatedly( Return( Chimera::Status::OK ) );

    /*-------------------------------------------------
    Verify-1
    -------------------------------------------------*/
    EXPECT_EQ( Chimera::Status::OK, initialize() );    // First time sets up the driver
    EXPECT_EQ( Chimera::Status::OK, initialize() );    // Second time is already initialized

    /*-------------------------------------------------
    Call FUT
    -------------------------------------------------*/
    EXPECT_EQ( Chimera::Status::OK, reset() );

    /*-------------------------------------------------
    Verify-2
    -------------------------------------------------*/
    EXPECT_EQ( Chimera::Status::OK, initialize() );    // First time sets up the driver
    EXPECT_EQ( Chimera::Status::OK, initialize() );    // Second time is already initialized
  }


  /*-------------------------------------------------------------------------------
  getDriver()
  -------------------------------------------------------------------------------*/
  TEST_F( CANModuleTest, GetDriver )
  {
    /*-------------------------------------------------
    Driver exists for CAN0
    -------------------------------------------------*/
    EXPECT_CALL( MockObj(), getResourceIndex( Chimera::CAN::Channel::CAN0 ) ).Times( Exactly( 1 ) ).WillOnce( Return( 0 ) );
    EXPECT_NE( nullptr, Thor::CAN::getDriver( Chimera::CAN::Channel::CAN0 ) );

    /*-------------------------------------------------
    Driver does not exist for CAN1
    -------------------------------------------------*/
    EXPECT_CALL( MockObj(), getResourceIndex( Chimera::CAN::Channel::CAN1 ) )
        .Times( Exactly( 1 ) )
        .WillOnce( Return( ::Thor::LLD::INVALID_RESOURCE_INDEX ) );
    EXPECT_EQ( nullptr, Thor::CAN::getDriver( Chimera::CAN::Channel::CAN1 ) );

    /*-------------------------------------------------
    Implentation of getResourceIndex() has a bug and
    returns a wild value
    -------------------------------------------------*/
    EXPECT_CALL( MockObj(), getResourceIndex( Chimera::CAN::Channel::CAN2 ) )
        .Times( Exactly( 1 ) )
        .WillOnce( Return( 39485 ) );
    EXPECT_EQ( nullptr, Thor::CAN::getDriver( Chimera::CAN::Channel::CAN2 ) );
  }


  /*-------------------------------------------------------------------------------
  getDriverShared()
  -------------------------------------------------------------------------------*/
  TEST_F( CANModuleTest, GetDriverShared )
  {
    /*-------------------------------------------------
    Register the shared driver instances
    -------------------------------------------------*/
    Thor::CAN::initialize();

    /*-------------------------------------------------
    Driver exists for CAN0
    -------------------------------------------------*/
    EXPECT_CALL( MockObj(), getResourceIndex( Chimera::CAN::Channel::CAN0 ) ).Times( Exactly( 1 ) ).WillOnce( Return( 0 ) );
    EXPECT_NE( nullptr, Thor::CAN::getDriverShared( Chimera::CAN::Channel::CAN0 ) );

    /*-------------------------------------------------
    Driver does not exist for CAN1
    -------------------------------------------------*/
    EXPECT_CALL( MockObj(), getResourceIndex( Chimera::CAN::Channel::CAN1 ) )
        .Times( Exactly( 1 ) )
        .WillOnce( Return( ::Thor::LLD::INVALID_RESOURCE_INDEX ) );
    EXPECT_EQ( nullptr, Thor::CAN::getDriverShared( Chimera::CAN::Channel::CAN1 ) );

    /*-------------------------------------------------
    Implentation of getResourceIndex() has a bug and
    returns a wild value
    -------------------------------------------------*/
    EXPECT_CALL( MockObj(), getResourceIndex( Chimera::CAN::Channel::CAN2 ) )
        .Times( Exactly( 1 ) )
        .WillOnce( Return( 5541 ) );
    EXPECT_EQ( nullptr, Thor::CAN::getDriverShared( Chimera::CAN::Channel::CAN2 ) );
  }
}    // namespace Thor::CAN
