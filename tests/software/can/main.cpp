/********************************************************************************
 *  File Name:
 *    main.cpp
 *
 *  Description:
 *    Entry to the CAN HLD tests
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#include <cstdio>
#include <gtest/gtest.h>

/* Chimera Includes */
#include <Thor/hld/can/hld_can_chimera.hpp>


int main( int argc, char **argv )
{
  Chimera::CAN::Backend::initialize();

  printf( "Running main() from %s\n", __FILE__ );
  testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}
