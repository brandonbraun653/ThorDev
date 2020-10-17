/********************************************************************************
 *  File Name:
 *    test_resources.cpp
 *
 *  Description:
 *    Implementation details of the STM32L4 CAN bus low level driver resources
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

/* Thor Includes */
#include <Thor/lld/interface/can/can_detail.hpp>


/* Test Includes */
#include "test_resources.hpp"

namespace Thor::LLD::CAN
{
  /*-------------------------------------------------------------------------------
  Private Data
  -------------------------------------------------------------------------------*/
  static MessageFilter filterList[ NUM_CAN_FILTER_BANKS ];

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
  static void assignFilter_32bListF0Active( MessageFilter &filter );

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  MessageFilter * cfgMsgFilterList( const FilterConfig cfgType )
  {
    switch( cfgType )
    {
      case FilterConfig::BASIC_VALID_FILTER:
        assignList_BasicValidFilter();
        return filterList;
        break;

      default:
        return nullptr;
        break;
    };
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

    CLEAR_ARRAY( filterList );
    for( auto x = 0; x < ARRAY_COUNT( filterList ); x++ )
    {
      filterList[ x ].active = false;
      filterList[ x ].mode   = FilterMode::UNKNOWN;
      filterList[ x ].scale  = FilterWidth::UNKNOWN;
    }
  }


  static void assignList_BasicValidFilter()
  {
    reset_filter();
    assignFilter_32bListF0Active( filterList[ 5 ] );
  }

  /*-------------------------------------------------
  Unit Filter Configuration
  -------------------------------------------------*/
  static void assignFilter_32bListF0Active( MessageFilter &filter )
  {
    using namespace Chimera::CAN;

    filter.active      = true;
    filter.fifoBank    = Mailbox::RX_MAILBOX_1;
    filter.mode        = FilterMode::ID_LIST;
    filter.scale       = FilterWidth::WIDTH_32BIT;
    filter.identifier  = 0;
    filter.mask        = 0;
    filter.assignedFMI = 0;
  }

}  // namespace Thor::LLD::CAN
