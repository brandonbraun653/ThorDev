/********************************************************************************
 *  File Name:
 *    test_resources.cpp
 *
 *  Description:
 *    Implementation details of the STM32L4 CAN bus low level driver resources
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Chimera Includes */
#include <Chimera/can>

/* Thor Includes */
#include <Thor/lld/interface/can/can_detail.hpp>
#include <Thor/lld/stm32l4x/can/hw_can_prv_driver.hpp>

/* Test Driver Includes */
#include "test_resources.hpp"

/* CppUTest Includes */
#include <CppUTest/TestHarness.h>

namespace Thor::LLD::CAN
{
  /*-------------------------------------------------------------------------------
  Private Data
  -------------------------------------------------------------------------------*/
  static MessageFilter asnFilterList[ NUM_CAN_MAX_FILTERS ];
  static MessageFilter expFilterList[ NUM_CAN_MAX_FILTERS ];
  static MessageFilter actFilterList[ NUM_CAN_MAX_FILTERS ];

  /*-------------------------------------------------------------------------------
  Static Function Declarations
  -------------------------------------------------------------------------------*/
  /*-------------------------------------------------
  List Filter Configuration
  -------------------------------------------------*/
  static void reset_filter();

  static void assignList_BasicValidFilter();

  /*-------------------------------------------------
  Unit Filter Configuration
  -------------------------------------------------*/
  static void assignFilter_32bListF0Active( const size_t asnPos, const size_t expPos, const size_t expFMI );

  static void assignFilter_16bMaskF1Active( const size_t asnPos, const size_t expPos, const size_t expFMI );

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  MessageFilter *cfgMsgFilterList( const FilterConfig cfgType )
  {
    switch ( cfgType )
    {
      case FilterConfig::BASIC_VALID_FILTER:
        assignList_BasicValidFilter();
        return asnFilterList;
        break;

      default:
        return nullptr;
        break;
    };
  }


  bool verifyFilterBankMatchesExpected( RegisterMap *const periph )
  {
    using namespace Chimera::CAN;

    /*-------------------------------------------------
    Rebuild the entire filter list from scratch
    -------------------------------------------------*/
    size_t bankIdx        = 0;
    size_t bankFltrIdx    = 0;
    size_t fltrIdx        = 0;
    uint32_t cachedFR1    = 0;
    uint32_t cachedFR2    = 0;
    FilterType cachedMode = FilterType::UNKNOWN;
    Mailbox cachedFIFO    = Mailbox::UNKNOWN;

    uint8_t fifo0FMI_current  = 0;
    uint8_t fifo0FMI_toAssign = 0;
    uint8_t fifo1FMI_current  = 0;
    uint8_t fifo1FMI_toAssign = 0;
    uint8_t *fifoCurrentPtr   = nullptr;
    uint8_t *fifoAssignPtr    = nullptr;
    bool bank_parsed          = false;

    do
    {
      /*-------------------------------------------------------------------------------
      First things first, the algorithm needs to figure out if the current filter bank
      contains any valid filters. This is accomplished by looking at the Mode, Scale,
      and an internal filter number counter that tracks found valid filters per bank.

      The tracker helps determine where to look in the filter bank for the next filter.
      -------------------------------------------------------------------------------*/
      /*-------------------------------------------------
      Parse the mode of the current filter bank
      -------------------------------------------------*/
      cachedMode = prv_get_filter_bank_mode( periph, bankIdx );

      /*-------------------------------------------------
      Parse the FIFO configuration
      -------------------------------------------------*/
      cachedFIFO = prv_get_filter_bank_fifo( periph, bankIdx );
      if ( cachedFIFO == Mailbox::RX_MAILBOX_1 )
      {
        fifoCurrentPtr = &fifo0FMI_current;
        fifoAssignPtr  = &fifo0FMI_toAssign;
      }
      else
      {
        fifoCurrentPtr = &fifo1FMI_current;
        fifoAssignPtr  = &fifo1FMI_toAssign;
      }

      /*-------------------------------------------------
      Reset the assignment data based on bank mode
      -------------------------------------------------*/
      if ( !bank_parsed )
      {
        switch ( cachedMode )
        {
          case FilterType::MODE_32BIT_MASK:
            *fifoAssignPtr = 1;
            break;

          case FilterType::MODE_32BIT_LIST:
          case FilterType::MODE_16BIT_MASK:
            *fifoAssignPtr = 2;
            break;

          case FilterType::MODE_16BIT_LIST:
            *fifoAssignPtr = 4;
            break;

          default:
            return false;
            break;
        }
      }

      /*-------------------------------------------------
      Cache the raw filter register values
      -------------------------------------------------*/
      cachedFR1 = periph->sFilterBank[ bankIdx ].FR1;
      cachedFR2 = periph->sFilterBank[ bankIdx ].FR2;

      /*-------------------------------------------------
      Now the ugly bit. Parse the filter bank based on
      the four mode configuration possibilities.
      -------------------------------------------------*/
      if ( cachedMode == FilterType::MODE_32BIT_MASK )
      {
        /*-------------------------------------------------
        Only a single filter can exist in this mode setup
        -------------------------------------------------*/
        if ( ( cachedFR1 == FLTR_RST_1 ) || ( cachedFR2 == FLTR_RST_2 ) )
        {
          // Data is in the reset state. No filter here!
          *fifoCurrentPtr++;
          bankIdx++;
          bankFltrIdx = 0;
          bank_parsed = false;
          continue;
        }
        else
        {
          // Got a good filter. Save off the mapped ID and mask.
          actFilterList[ fltrIdx ].identifier = cachedFR1;
          actFilterList[ fltrIdx ].mask       = cachedFR2;
        }
      }
      else if ( cachedMode == FilterType::MODE_32BIT_LIST )
      {
        /*-------------------------------------------------
        Update the tracking metrics for which filter inside
        this bank the algorithm is currently on.
        -------------------------------------------------*/
        if ( bankFltrIdx >= 2 )
        {
          // Max filter hit. Need to go to the next bank.
          bankFltrIdx = 0;
          bankIdx++;
          bank_parsed = false;
          continue;
        }

        /*-------------------------------------------------
        Check the validity of the bank data
        -------------------------------------------------*/
        bool validFilter    = false;
        uint32_t validValue = 0;

        switch ( bankFltrIdx )
        {
          case 0:
            validValue  = cachedFR1;
            validFilter = ( cachedFR1 != FLTR_RST_1 );
            break;

          case 1:
            validValue  = cachedFR2;
            validFilter = ( cachedFR2 != FLTR_RST_2 );
            break;

          default:
            return false;
            break;
        }

        bankFltrIdx++;

        /*-------------------------------------------------
        Assuming the filter has good data, save off the id
        and mask, then go parse the rest of the data.
        -------------------------------------------------*/
        if ( validFilter )
        {
          actFilterList[ fltrIdx ].identifier = validValue;
          actFilterList[ fltrIdx ].mask       = 0;
        }
        else
        {
          *fifoCurrentPtr++;
          continue;    // Move on and try the next filter location
        }
      }
      else if ( cachedMode == FilterType::MODE_16BIT_MASK )
      {
        /*-------------------------------------------------
        Update bank filter slot tracking
        -------------------------------------------------*/
        if ( bankFltrIdx >= 2 )
        {
          *fifoCurrentPtr++;
          bankIdx++;
          bankFltrIdx = 0;
          bank_parsed = false;
          continue;
        }

        /*-------------------------------------------------
        Check the validity of the bank data
        -------------------------------------------------*/
        bool validFilter   = false;
        uint32_t validID   = 0;
        uint32_t validMask = 0;

        switch ( bankFltrIdx )
        {
          case 0:
            validID   = ( cachedFR1 & 0x0000FFFF );
            validMask = ( cachedFR1 & 0xFFFF0000 );

            if ( ( validID != ( FLTR_RST_1 & 0x0000FFFF ) ) && ( validMask != ( FLTR_RST_1 & 0xFFFF0000 ) ) )
            {
              validFilter = true;
            }
            break;

          case 1:
            validID   = ( cachedFR2 & 0x0000FFFF );
            validMask = ( cachedFR2 & 0xFFFF0000 );

            if ( ( validID != ( FLTR_RST_2 & 0x0000FFFF ) ) && ( validMask != ( FLTR_RST_2 & 0xFFFF0000 ) ) )
            {
              validFilter = true;
            }
            break;

          default:
            return false;
            break;
        }

        bankFltrIdx++;

        /*-------------------------------------------------
        Assuming the filter has good data, save off the id
        and mask, then go parse the rest of the data.
        -------------------------------------------------*/
        if ( validFilter )
        {
          actFilterList[ fltrIdx ].identifier = validID;
          actFilterList[ fltrIdx ].mask       = validMask >> 16;
        }
        else
        {
          *fifoCurrentPtr++;
          continue;    // Move on and try the next filter location
        }
      }
      else if ( cachedMode == FilterType::MODE_16BIT_LIST )
      {
        /*-------------------------------------------------
        Update bank filter slot tracking
        -------------------------------------------------*/
        if ( bankFltrIdx >= 4 )
        {
          *fifoCurrentPtr++;
          bankIdx++;
          bankFltrIdx = 0;
          bank_parsed = false;
          continue;
        }

        /*-------------------------------------------------
        Check the validity of the bank data
        -------------------------------------------------*/
        bool validFilter = false;
        uint32_t validID = 0;

        switch ( bankFltrIdx )
        {
          case 0:
            validID     = ( cachedFR1 & 0x0000FFFF );
            validFilter = ( validID != ( FLTR_RST_1 & 0x0000FFFF ) );
            break;

          case 1:
            validID     = ( ( cachedFR1 & 0xFFFF0000 ) >> 16 );
            validFilter = ( validID != ( ( FLTR_RST_1 & 0xFFFF0000 ) >> 16 ) );
            break;

          case 2:
            validID     = ( cachedFR2 & 0x0000FFFF );
            validFilter = ( validID != ( FLTR_RST_2 & 0x0000FFFF ) );
            break;

          case 3:
            validID     = ( ( cachedFR2 & 0xFFFF0000 ) >> 16 );
            validFilter = ( validID != ( ( FLTR_RST_2 & 0xFFFF0000 ) >> 16 ) );
            break;

          default:
            return false;
            break;
        }

        bankFltrIdx++;

        /*-------------------------------------------------
        Assuming the filter has good data, save off the id
        and mask, then go parse the rest of the data.
        -------------------------------------------------*/
        if ( validFilter )
        {
          actFilterList[ fltrIdx ].identifier = validID;
          actFilterList[ fltrIdx ].mask       = 0;
        }
        else
        {
          *fifoCurrentPtr++;
          continue;    // Move on and try the next filter location
        }
      }

      /*-------------------------------------------------------------------------------
      Getting to this point means a valid filter has been discovered and its config
      information should be added to the "actual" filter cache.
      -------------------------------------------------------------------------------*/
      /*-------------------------------------------------
      Save off the cached data. No need to look these up.
      -------------------------------------------------*/
      actFilterList[ fltrIdx ].valid      = true;
      actFilterList[ fltrIdx ].filterType = cachedMode;
      actFilterList[ fltrIdx ].hwFMI      = *fifoCurrentPtr;

      *fifoCurrentPtr++;
      *fifoAssignPtr--;

      /*-------------------------------------------------
      Parse the FIFO assignment register
      -------------------------------------------------*/
      if ( periph->FFA1R & ( 1u << bankIdx ) )
      {
        actFilterList[ fltrIdx ].fifoBank = CAN::Mailbox::RX_MAILBOX_2;
      }
      else
      {
        actFilterList[ fltrIdx ].fifoBank = CAN::Mailbox::RX_MAILBOX_1;
      }

      /*-------------------------------------------------
      Parse the activation register
      -------------------------------------------------*/
      actFilterList[ fltrIdx ].active = static_cast<bool>( periph->FA1R & ( 1u << bankIdx ) );

      /*-------------------------------------------------
      Update the tracking data
      -------------------------------------------------*/
      fltrIdx++;
      bank_parsed = true;

    } while ( ( fltrIdx < NUM_CAN_MAX_FILTERS ) && ( bankIdx < NUM_CAN_FILTER_BANKS ) );

    /*-------------------------------------------------
    Compare actual and expected to make sure they match
    -------------------------------------------------*/
    bool compareResult = true;

    for ( auto idx = 0; idx < NUM_CAN_MAX_FILTERS; idx++ )
    {
      /*-------------------------------------------------
      If the filter is not active in hardware or the
      reconstruction process marked the slot as invalid,
      don't process the configuration.
      -------------------------------------------------*/
      if ( !actFilterList[ idx ].active || !actFilterList[ idx ].valid )
      {
        continue;
      }

      /*-------------------------------------------------
      Otherwise make sure the configs match the expected
      -------------------------------------------------*/
      if ( memcmp( &expFilterList[ idx ], &actFilterList[ idx ], sizeof( MessageFilter ) ) != 0 )
      {
        // Explicit assignment to make it easier to set breakpoints for debugging
        compareResult = false;
        break;
      }
    }

    return compareResult;
  }


  bool verifyFramesMatch( const Chimera::CAN::BasicFrame &frame1, const Chimera::CAN::BasicFrame &frame2 )
  {
    /*-------------------------------------------------
    Compare everything except the filter match index.
    That aspect is assigned via hardware and is very
    dynamic.
    -------------------------------------------------*/
    /* clang-format off */
    auto attributes_equal = (
      ( frame1.dataLength == frame2.dataLength  ) &&
      ( frame1.frameType  == frame2.frameType   ) &&
      ( frame1.id         == frame2.id          ) &&
      ( frame1.idMode     == frame2.idMode      )
    );
    /* clang-format on */
    if( !attributes_equal )
    {
      return false;
    }

    // Compare the data fields now that we know the lengths are equal
    for ( auto x = 0; x < frame1.dataLength; x++ )
    {
      if ( frame1.data[ x ] != frame2.data[ x ] )
      {
        return false;
      }
    }

    return true;
  }

  /*-------------------------------------------------------------------------------
  Static Function Definitions
  -------------------------------------------------------------------------------*/
  /*-------------------------------------------------
  List Filter Configuration
  -------------------------------------------------*/
  static void reset_filter()
  {
    using namespace Chimera::CAN;

    /*-------------------------------------------------
    Default test filter configuration
    -------------------------------------------------*/
    CLEAR_ARRAY( expFilterList );
    for ( auto x = 0; x < ARRAY_COUNT( expFilterList ); x++ )
    {
      expFilterList[ x ].valid      = true;
      expFilterList[ x ].active     = false;
      expFilterList[ x ].filterType = FilterType::MODE_16BIT_MASK;
      expFilterList[ x ].fifoBank   = CAN::Mailbox::RX_MAILBOX_1;
      expFilterList[ x ].hwFMI      = 0;
      expFilterList[ x ].identifier = 0;
      expFilterList[ x ].mask       = 0;

      actFilterList[ x ].valid      = false;
      actFilterList[ x ].active     = false;
      actFilterList[ x ].filterType = FilterType::UNKNOWN;
      actFilterList[ x ].fifoBank   = CAN::Mailbox::UNKNOWN;
      actFilterList[ x ].hwFMI      = 0;
      actFilterList[ x ].identifier = 0;
      actFilterList[ x ].mask       = 0;
    }
  }


  static void assignList_BasicValidFilter()
  {
    reset_filter();
    assignFilter_32bListF0Active( 5, 0, 0 );
    assignFilter_16bMaskF1Active( 3, 1, 0 );
  }


  /*-------------------------------------------------
  Unit Filter Configuration
  -------------------------------------------------*/
  static void assignFilter_32bListF0Active( const size_t asnPos, const size_t expPos, const size_t expFMI )
  {
    using namespace Chimera::CAN;

    // Configure the input filter's parameters for the test
    asnFilterList[ asnPos ].valid      = true;
    asnFilterList[ asnPos ].active     = true;
    asnFilterList[ asnPos ].fifoBank   = Mailbox::RX_MAILBOX_1;
    asnFilterList[ asnPos ].filterType = FilterType::MODE_32BIT_LIST;
    asnFilterList[ asnPos ].identifier = 0;
    asnFilterList[ asnPos ].mask       = 0;
    asnFilterList[ asnPos ].hwFMI      = expFMI;

    // Copy the input data to the expected output location
    expFilterList[ expPos ] = asnFilterList[ asnPos ];
  }


  static void assignFilter_16bMaskF1Active( const size_t asnPos, const size_t expPos, const size_t expFMI )
  {
    using namespace Chimera::CAN;

    // Configure the input filter's parameters for the test
    asnFilterList[ asnPos ].valid      = true;
    asnFilterList[ asnPos ].active     = true;
    asnFilterList[ asnPos ].fifoBank   = Mailbox::RX_MAILBOX_2;
    asnFilterList[ asnPos ].filterType = FilterType::MODE_16BIT_MASK;
    asnFilterList[ asnPos ].identifier = 0x00001567;
    asnFilterList[ asnPos ].mask       = 0x0000FFFF;
    asnFilterList[ asnPos ].hwFMI      = expFMI;

    // Copy the input data to the expected output location
    expFilterList[ expPos ] = asnFilterList[ asnPos ];
  }

}    // namespace Thor::LLD::CAN
