/********************************************************************************
 *  File Name:
 *    can_test_fixture.hpp
 *
 *  Description:
 *    Fixtures for testing CAN HLD
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef THOR_HLD_CAN_TESTING_HPP
#define THOR_HLD_CAN_TESTING_HPP

/* Thor Includes */
#include <Thor/can>
#include <Thor/lld/interface/can/can_intf.hpp>
#include <Thor/lld/interface/can/can_detail.hpp>

/* Testing Includes */
#include <gtest/gtest.h>

namespace Thor::CAN
{

  class CANModuleTest : public ::testing::Test
  {
  protected:
    void SetUp() override
    {
      ASSERT_TRUE( ::testing::Mock::VerifyAndClearExpectations( &MockObj() ) );
      Thor::CAN::reset();
    }

    void TearDown() override
    {
    }

    Thor::LLD::CAN::Mock::NiceModuleMock &MockObj()
    {
      return Thor::LLD::CAN::Mock::getModuleMockObject();
    }
  };


  class CANDriverTest : public ::testing::Test
  {
  protected:
    void SetUp() override
    {
      ASSERT_TRUE( ::testing::Mock::VerifyAndClearExpectations( &DriverMock( Chimera::CAN::Channel::CAN0 ) ) );
      ASSERT_TRUE( ::testing::Mock::VerifyAndClearExpectations( &ModuleMock() ) );
      Thor::CAN::reset();
    }

    void TearDown() override
    {
    }


    Thor::LLD::CAN::Mock::NiceModuleMock &ModuleMock()
    {
      return Thor::LLD::CAN::Mock::getModuleMockObject();
    }

    Thor::LLD::CAN::Mock::NiceDriverMock &DriverMock( const Chimera::CAN::Channel channel )
    {
      return Thor::LLD::CAN::Mock::getDriverMockObject( channel );
    }
  };
}  // namespace Thor::CAN

#endif  /* !THOR_HLD_CAN_TESTING_HPP */
