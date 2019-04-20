/********************************************************************************
 *  File Name:
 *    system_flash_test.cpp
 *
 *  Description:
 *    Embedded tests for the Thor internal system flash driver 
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <algorithm>
#include <array>
#include <cstdint>

/* Test Driver Includes */
#include "TinyEmbeddedTest.h"

/* Thor Includes */
#include <Thor/flash.hpp>

using namespace Thor::Memory;

static std::array<uint8_t, 1024> writeData;
static std::array<uint8_t, 1024> readData;
static InternalFlash flash;

static constexpr uint8_t FLASH_RESET_VAL = 0xFF;

/*------------------------------------------------
Local Functions
------------------------------------------------*/
static void reset_test();

static void checkAddressToSector();

/*------------------------------------------------
Verify the memory helper functions
------------------------------------------------*/
TEST_GROUP( HelperFunctions ){};

TEST( HelperFunctions, programWidth )
{
  CHECK_EQUAL( 1, flash.getPgmByteWidth( FLASH_TYPEPROGRAM_BYTE ) );
  CHECK_EQUAL( 2, flash.getPgmByteWidth( FLASH_TYPEPROGRAM_HALFWORD ) );
  CHECK_EQUAL( 4, flash.getPgmByteWidth( FLASH_TYPEPROGRAM_WORD ) );
  CHECK_EQUAL( 8, flash.getPgmByteWidth( FLASH_TYPEPROGRAM_DOUBLEWORD ) );
}

TEST( HelperFunctions, convertHALError )
{
  using namespace Chimera::Modules::Memory;
  CHECK_EQUAL( Status::OK, flash.convertHALError( HAL_FLASH_ERROR_NONE ) );
  CHECK_EQUAL( Status::FAIL, flash.convertHALError( HAL_FLASH_ERROR_OPERATION ) );
  CHECK_EQUAL( Status::ERR_READ_PROTECT, flash.convertHALError( HAL_FLASH_ERROR_RD ) );
  CHECK_EQUAL( Status::ERR_PGM_SEQUENCE, flash.convertHALError( HAL_FLASH_ERROR_PGS ) );
  CHECK_EQUAL( Status::ERR_PGM_PARALLEL, flash.convertHALError( HAL_FLASH_ERROR_PGP ) );
  CHECK_EQUAL( Status::ERR_PGM_ALIGNMENT, flash.convertHALError( HAL_FLASH_ERROR_PGA ) );
  CHECK_EQUAL( Status::ERR_WRITE_PROTECT, flash.convertHALError( HAL_FLASH_ERROR_WRP ) );
  CHECK_EQUAL( Status::UNKNOWN_ERROR, flash.convertHALError( std::numeric_limits<uint32_t>::max() ) );
}

TEST( HelperFunctions, addressToSector )
{
  checkAddressToSector();
}

/*------------------------------------------------
Verify the actual Flash memory interface behavior
------------------------------------------------*/
/* clang-format off */
TEST_GROUP( InternalFlash )
{
  public:
    void teardown() final override
    {
      reset_test();
    }
};
/* clang-format on */

TEST( InternalFlash, SectorErase )
{
  uint32_t address;
  
#if defined(STM32F446xx)
  address = FLASH_SECTOR_7_START_ADDR;
#else
#error Unknown Device
#endif
  
  reset_test();

  CHECK_EQUAL( Chimera::CommonStatusCodes::OK, flash.erase( address, 10 ) );
  CHECK_EQUAL( Chimera::CommonStatusCodes::OK, flash.read( address, readData.data(), readData.size() ) );
  CHECK( std::all_of( readData.begin(), readData.end(), []( const uint8_t x ) { return x == FLASH_RESET_VAL; } ) );
}

TEST( InternalFlash, ReadWriteErase )
{
  uint32_t address;
  
#if defined(STM32F446xx)
  address = FLASH_SECTOR_7_START_ADDR;
#else
#error Unknown Device
#endif
  
  reset_test();
  
  /* Read out the erased region, setting readData to 0xFF */
  CHECK_EQUAL( Chimera::CommonStatusCodes::OK, flash.erase( address, 10 ) );
  CHECK_EQUAL( Chimera::CommonStatusCodes::OK, flash.read( address, readData.data(), readData.size() ) );
  
  /* Now call FUT and clear the erased region by writing all 0x00 */
  CHECK_EQUAL( Chimera::CommonStatusCodes::OK, flash.write( address, writeData.data(), writeData.size() ) );

  /* Read it back out again, and verify everything was cleared */  
  CHECK_EQUAL( Chimera::CommonStatusCodes::OK, flash.read( address, readData.data(), readData.size() ) );
  CHECK( std::all_of( readData.begin(), readData.end(), []( const uint8_t x ) { return x == 0x00; } ) );
}


static void reset_test()
{
  memset( writeData.data(), 0, writeData.size() );
  memset( readData.data(), 0, readData.size() );
}

/*------------------------------------------------
Device specific function implementations
------------------------------------------------*/
#if defined( STM32F446xx )

static void checkAddressToSector()
{
  uint32_t sectorNum;
  
  /*------------------------------------------------
  Sector 0
  ------------------------------------------------*/
  sectorNum = 0u;
  
  CHECK_EQUAL( FLASH_SECTOR_0_START_ADDR, sectorStartBoundary[ sectorNum ] );
  CHECK_EQUAL( ( FLASH_SECTOR_0_END_ADDR - FLASH_SECTOR_0_START_ADDR ), FLASH_SECTOR_0_SIZE );
  
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_0_START_ADDR ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_0_START_ADDR - 1 ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_0_END_ADDR ) );
  CHECK_EQUAL( sectorNum + 1, flash.addressToSector( FLASH_SECTOR_0_END_ADDR + 1 ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_0_START_ADDR + ( FLASH_SECTOR_0_SIZE / 2 ) ) );
  
  /*------------------------------------------------
  Sector 1
  ------------------------------------------------*/
  sectorNum = 1u;
  
  CHECK_EQUAL( FLASH_SECTOR_1_START_ADDR, sectorStartBoundary[ sectorNum ] );
  CHECK_EQUAL( ( FLASH_SECTOR_1_END_ADDR - FLASH_SECTOR_1_START_ADDR ), FLASH_SECTOR_1_SIZE );
  
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_1_START_ADDR ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_1_END_ADDR ) );
  CHECK_EQUAL( sectorNum - 1, flash.addressToSector( FLASH_SECTOR_1_START_ADDR - 1 ) );
  CHECK_EQUAL( sectorNum + 1, flash.addressToSector( FLASH_SECTOR_1_END_ADDR + 1 ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_1_START_ADDR + ( FLASH_SECTOR_1_SIZE / 2 ) ) );
  
  /*------------------------------------------------
  Sector 2
  ------------------------------------------------*/
  sectorNum = 2u;
  
  CHECK_EQUAL( FLASH_SECTOR_2_START_ADDR, sectorStartBoundary[ sectorNum ] );
  CHECK_EQUAL( ( FLASH_SECTOR_2_END_ADDR - FLASH_SECTOR_2_START_ADDR ), FLASH_SECTOR_2_SIZE );
  
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_2_START_ADDR ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_2_END_ADDR ) );
  CHECK_EQUAL( sectorNum - 1, flash.addressToSector( FLASH_SECTOR_2_START_ADDR - 1 ) );
  CHECK_EQUAL( sectorNum + 1, flash.addressToSector( FLASH_SECTOR_2_END_ADDR + 1 ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_2_START_ADDR + ( FLASH_SECTOR_2_SIZE / 2 ) ) );
  
  /*------------------------------------------------
  Sector 3
  ------------------------------------------------*/
  sectorNum = 3u;
  
  CHECK_EQUAL( FLASH_SECTOR_3_START_ADDR, sectorStartBoundary[ sectorNum ] );
  CHECK_EQUAL( ( FLASH_SECTOR_3_END_ADDR - FLASH_SECTOR_3_START_ADDR ), FLASH_SECTOR_3_SIZE );
  
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_3_START_ADDR ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_3_END_ADDR ) );
  CHECK_EQUAL( sectorNum - 1, flash.addressToSector( FLASH_SECTOR_3_START_ADDR - 1 ) );
  CHECK_EQUAL( sectorNum + 1, flash.addressToSector( FLASH_SECTOR_3_END_ADDR + 1 ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_3_START_ADDR + ( FLASH_SECTOR_3_SIZE / 2 ) ) );
  
  /*------------------------------------------------
  Sector 4
  ------------------------------------------------*/
  sectorNum = 4u;
  
  CHECK_EQUAL( FLASH_SECTOR_4_START_ADDR, sectorStartBoundary[ sectorNum ] );
  CHECK_EQUAL( ( FLASH_SECTOR_4_END_ADDR - FLASH_SECTOR_4_START_ADDR ), FLASH_SECTOR_4_SIZE );
  
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_4_START_ADDR ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_4_END_ADDR ) );
  CHECK_EQUAL( sectorNum - 1, flash.addressToSector( FLASH_SECTOR_4_START_ADDR - 1 ) );
  CHECK_EQUAL( sectorNum + 1, flash.addressToSector( FLASH_SECTOR_4_END_ADDR + 1 ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_4_START_ADDR + ( FLASH_SECTOR_4_SIZE / 2 ) ) );
  
  /*------------------------------------------------
  Sector 5
  ------------------------------------------------*/
  sectorNum = 5u;
  
  CHECK_EQUAL( FLASH_SECTOR_5_START_ADDR, sectorStartBoundary[ sectorNum ] );
  CHECK_EQUAL( ( FLASH_SECTOR_5_END_ADDR - FLASH_SECTOR_5_START_ADDR ), FLASH_SECTOR_5_SIZE );
  
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_5_START_ADDR ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_5_END_ADDR ) );
  CHECK_EQUAL( sectorNum - 1, flash.addressToSector( FLASH_SECTOR_5_START_ADDR - 1 ) );
  CHECK_EQUAL( sectorNum + 1, flash.addressToSector( FLASH_SECTOR_5_END_ADDR + 1 ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_5_START_ADDR + ( FLASH_SECTOR_5_SIZE / 2 ) ) );
  
  /*------------------------------------------------
  Sector 6
  ------------------------------------------------*/
  sectorNum = 6u;
  
  CHECK_EQUAL( FLASH_SECTOR_6_START_ADDR, sectorStartBoundary[ sectorNum ] );
  CHECK_EQUAL( ( FLASH_SECTOR_6_END_ADDR - FLASH_SECTOR_6_START_ADDR ), FLASH_SECTOR_6_SIZE );
  
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_6_START_ADDR ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_6_END_ADDR ) );
  CHECK_EQUAL( sectorNum - 1, flash.addressToSector( FLASH_SECTOR_6_START_ADDR - 1 ) );
  CHECK_EQUAL( sectorNum + 1, flash.addressToSector( FLASH_SECTOR_6_END_ADDR + 1 ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_6_START_ADDR + ( FLASH_SECTOR_6_SIZE / 2 ) ) );
  
  /*------------------------------------------------
  Sector 7
  ------------------------------------------------*/
  sectorNum = 7u;
  
  CHECK_EQUAL( FLASH_SECTOR_7_START_ADDR, sectorStartBoundary[ sectorNum ] );
  CHECK_EQUAL( ( FLASH_SECTOR_7_END_ADDR - FLASH_SECTOR_7_START_ADDR ), FLASH_SECTOR_7_SIZE );
  
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_7_START_ADDR ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_7_END_ADDR ) );
  CHECK_EQUAL( sectorNum - 1, flash.addressToSector( FLASH_SECTOR_7_START_ADDR - 1 ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_7_END_ADDR + 1 ) );
  CHECK_EQUAL( sectorNum, flash.addressToSector( FLASH_SECTOR_7_START_ADDR + ( FLASH_SECTOR_7_SIZE / 2 ) ) );
}

#else
#error Unknown device. Cannot execute system flash memory tests.
#endif