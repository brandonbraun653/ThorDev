/********************************************************************************
 *  File Name:
 *    test_resources.hpp
 *
 *  Description:
 *    Resources used in testing the STM32L4 CAN bus low level driver
 *
 *  2020 | Brandon Braun | brandonbraun653@gmail.com
 *******************************************************************************/

#pragma once
#ifndef STM32L4_CAN_LLD_TEST_RESOURCES_HPP
#define STM32L4_CAN_LLD_TEST_RESOURCES_HPP

/* Thor Includes */
#include <Thor/lld/interface/can/can_types.hpp>

namespace Thor::LLD::CAN
{
  /*-------------------------------------------------------------------------------
  Enumerations
  -------------------------------------------------------------------------------*/
  enum class FilterConfig : uint8_t
  {
    BASIC_VALID_FILTER,  /**< Single filter is configured, rest are inactive */

    NUM_OPTIONS,
    UNKNOWN
  };

  /*-------------------------------------------------------------------------------
  Public Functions
  -------------------------------------------------------------------------------*/
  /**
   *  Initializes an array of filters to be configured according
   *  to the given config type.
   *
   *  @note The array will be sized according to the number of filter
   *        banks supported by the hardware.
   *
   *  @param[in]  cfgType       Which configuration to apply
   *  @return MessageFilter*    The configured list
   */
  MessageFilter * cfgMsgFilterList( const FilterConfig cfgType );

  /**
   *  Validates that the current configuration of the filter
   *  bank registers match the expected values. The expected values
   *  are modified based on the input of cfgMsgFilterList()
   *
   *  @param[in]  periph        The peripheral instance to check
   *  @return bool              True if the config matched, false if not
   */
  bool verifyFilterBankMatchesExpected( RegisterMap *const periph );

}  // namespace Thor::LLD::CAN

#endif  /* !STM32L4_CAN_LLD_TEST_RESOURCES_HPP */
