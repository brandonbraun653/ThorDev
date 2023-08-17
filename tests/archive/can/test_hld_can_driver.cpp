/********************************************************************************
 *  File Name:
 *    test_hld_can_driver.cpp
 *
 *  Description:
 *    Tests the HLD CAN driver
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* STL Includes */
#include <array>
#include <vector>

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
  using ::testing::AtLeast;
  using ::testing::Exactly;
  using ::testing::Return;
  using ::testing::_;

  /*-------------------------------------------------------------------------------
  Static Functions & Data
  -------------------------------------------------------------------------------*/

  static std::array<Chimera::CAN::BasicFrame, 5> s_tx_buffer;
  static std::array<Chimera::CAN::BasicFrame, 5> s_rx_buffer;

  static Chimera::CAN::DriverConfig getValidConfig()
  {
    Chimera::CAN::DriverConfig cfg;
    cfg.clear();
    cfg.validity = true;

    /*-------------------------------------------------
    GPIO Configuration
    -------------------------------------------------*/
    cfg.TXInit.alternate = Chimera::GPIO::Alternate::CAN1_TX;
    cfg.TXInit.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
    cfg.TXInit.pin       = 12;
    cfg.TXInit.port      = Chimera::GPIO::Port::PORTA;
    cfg.TXInit.pull      = Chimera::GPIO::Pull::NO_PULL;
    cfg.TXInit.threaded  = true;
    cfg.TXInit.validity  = true;

    cfg.RXInit.alternate = Chimera::GPIO::Alternate::CAN1_RX;
    cfg.RXInit.drive     = Chimera::GPIO::Drive::ALTERNATE_PUSH_PULL;
    cfg.RXInit.pin       = 11;
    cfg.RXInit.port      = Chimera::GPIO::Port::PORTA;
    cfg.RXInit.pull      = Chimera::GPIO::Pull::NO_PULL;
    cfg.RXInit.threaded  = true;
    cfg.RXInit.validity  = true;

    /*-------------------------------------------------
    CAN Parameter Configuration
    -------------------------------------------------*/
    cfg.HWInit.baudRate           = 100000;
    cfg.HWInit.channel            = Chimera::CAN::Channel::CAN0;
    cfg.HWInit.maxBaudError       = 0.05f;
    cfg.HWInit.resyncJumpWidth    = 1;
    cfg.HWInit.samplePointPercent = 0.85f;
    cfg.HWInit.timeQuanta         = 16;
    cfg.HWInit.rxBuffer           = s_rx_buffer.data();
    cfg.HWInit.rxElements         = s_rx_buffer.size();
    cfg.HWInit.txBuffer           = s_tx_buffer.data();
    cfg.HWInit.txElements         = s_tx_buffer.size();

    return cfg;
  }


  /*-------------------------------------------------------------------------------
  Driver::open()
  -------------------------------------------------------------------------------*/
  TEST_F( CANDriverTest, Open_BadConfig )
  {
    /*-------------------------------------------------
    Initialize
    -------------------------------------------------*/
    std::vector<Chimera::CAN::DriverConfig> testConfigs;
    std::array<Chimera::CAN::BasicFrame, 5> txBuffer;
    std::array<Chimera::CAN::BasicFrame, 5> rxBuffer;

    Chimera::CAN::DriverConfig cfg;
    cfg.clear();

    // Empty config
    testConfigs.push_back( cfg );

    // Config with no validity
    cfg.validity = false;
    testConfigs.push_back( cfg );

    // Config with partial validities
    cfg.validity        = true;
    cfg.RXInit.validity = false;
    cfg.TXInit.validity = true;
    testConfigs.push_back( cfg );

    cfg.RXInit.validity = true;
    cfg.TXInit.validity = false;
    testConfigs.push_back( cfg );

    // Config with missing buffers
    cfg.TXInit.validity = true;
    cfg.HWInit.rxBuffer = nullptr;
    testConfigs.push_back( cfg );

    cfg.HWInit.rxBuffer = rxBuffer.data();
    cfg.HWInit.txBuffer = nullptr;
    testConfigs.push_back( cfg );

    // Config with bad element sizing for buffers
    cfg.HWInit.txBuffer   = txBuffer.data();
    cfg.HWInit.rxElements = 0;
    testConfigs.push_back( cfg );

    cfg.HWInit.rxElements = rxBuffer.size();
    cfg.HWInit.txElements = 0;
    testConfigs.push_back( cfg );

    // Config with bad channel
    cfg.HWInit.txElements = txBuffer.size();
    cfg.HWInit.channel    = Chimera::CAN::Channel::UNKNOWN;
    testConfigs.push_back( cfg );

    // Setup the LLD to call into channel 0
    EXPECT_CALL( ModuleMock(), getResourceIndex( TestChannel ) )
        .Times( AtLeast( 1 ) )
        .WillRepeatedly( Return( 0 ) );

    /*-------------------------------------------------
    Verify
    -------------------------------------------------*/
    auto driver = Thor::CAN::getDriverShared( TestChannel );

    for ( auto &config : testConfigs )
    {
      EXPECT_EQ( Chimera::Status::INVAL_FUNC_PARAM, driver->open( config ) );
    }
  }


  TEST_F( CANDriverTest, Open_LLDInitFailure )
  {
    /*-------------------------------------------------
    Initialize
    -------------------------------------------------*/
    Thor::CAN::initialize();

    EXPECT_CALL( ModuleMock(), getDriver( _ ) ).WillOnce( Return( nullptr ) );
    EXPECT_CALL( DriverMock( TestChannel ), configure( _ ) ).WillOnce( Return( Chimera::Status::FAILED_INIT ) );

    auto driver = Thor::CAN::getDriverShared( TestChannel );

    /*-------------------------------------------------
    Verify
    -------------------------------------------------*/
    ASSERT_NE( nullptr, driver );
    EXPECT_EQ( Chimera::Status::FAILED_INIT, driver->open( getValidConfig() ) );
  }


  /*-------------------------------------------------------------------------------
  Driver::send()
  -------------------------------------------------------------------------------*/
  TEST_F( CANDriverTest, SendFrame )
  {

  }
}    // namespace Thor::CAN
