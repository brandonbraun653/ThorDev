/********************************************************************************
 *  File Name:
 *    test_hld_can_chimera.cpp
 *
 *	 Description:
 *    Tests the Chimera interface to Thor CAN
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#if defined( THOR_HLD_TEST )
/* GTest Includes */
#include "gtest/gtest.h"

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/can>

/* Thor Includes */
#include <Thor/cfg>
#include <Thor/can>
#include <Thor/hld/can/hld_can_chimera.hpp>


TEST(PleasePass, PrettyPlease)
{
  EXPECT_EQ(0, 0);

  Chimera::CAN::Backend::initialize();
}

#endif