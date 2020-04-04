# **********************************************************************************************************************
#   FileName:
#       bp_mapping.py
#
#   Description:
#       Boilerplate generator for LLD mapping files
#
#   Usage Examples:
#       N/A
#
#   2020 | Brandon Braun | brandonbraun653@gmail.com
# **********************************************************************************************************************

from generators.lld_templates.bp_common import BaseAttributes


class LLDMappingTemplate(BaseAttributes):
    """ Generates starter header/source data for LLD mapping files """

    @property
    def has_header_file(self):
        return True

    @property
    def has_source_file(self):
        return True

    @property
    def include_guard(self):
        return "THOR_HW_" + self.driver.upper() + "_MAPPING_HPP"

    def generate_header_filename(self):
        return "hw_" + self.driver.lower() + "_mapping.hpp"

    def generate_header_text(self):
        template = self.__header_template()
        return template.format(periph_lc=self.driver.lower(), periph_uc=self.driver.upper(),
                               device_lc=self.device.lower(), device_uc=self.device.upper(),
                               family_uc=self.family.upper(), family_lc=self.family.lower(),
                               chip_uc=self.chip.upper(), chip_lc=self.chip.lower(),
                               year=self.year, author=self.author, email=self.email,
                               include_guard=self.include_guard)

    def generate_source_filename(self):
        return "hw_" + self.driver.lower() + "_mapping.cpp"

    def generate_source_text(self):
        template = self.__module_template()
        return template.format(periph_lc=self.driver.lower(), periph_uc=self.driver.upper(),
                               device_lc=self.device.lower(), device_uc=self.device.upper(),
                               family_uc=self.family.upper(), family_lc=self.family.lower(),
                               chip_uc=self.chip.upper(), chip_lc=self.chip.lower(),
                               year=self.year, author=self.author, email=self.email,
                               include_guard=self.include_guard)

    @staticmethod
    def __header_template():
        return \
"""/********************************************************************************
 *  File Name:
 *    hw_{periph_lc}_mapping.hpp
 *
 *  Description:
 *    Provides structures for conversion and mapping between data types for fast
 *    runtime performance of driver code.
 *
 *  {year} | {author} | {email}
 ********************************************************************************/

#pragma once
#ifndef {include_guard}
#define {include_guard}

/* STL Includes */
#include <array>

/* Chimera Includes */
#include <Chimera/container>
#include <Chimera/{periph_lc}>

/* Driver Includes */
#include <Thor/lld/interface/{periph_lc}/{periph_lc}_types.hpp>
#include <Thor/lld/{family_lc}x/rcc/hw_rcc_types.hpp>
#include <Thor/lld/{family_lc}x/{periph_lc}/hw_{periph_lc}_types.hpp>

namespace Thor::LLD::{periph_uc}
{{
  /*-------------------------------------------------
  Define Memory Mapped Structs for Peripheral
  -------------------------------------------------*/
// #if defined( STM32_GPIOA_PERIPH_AVAILABLE )
//   extern RegisterMap *GPIOA_PERIPH;
// #endif
//
// ...  Additional Definitions

  /*------------------------------------------------
  Hardware Memory Mappings
  ------------------------------------------------*/
  extern PeriphRegisterList PeripheralList;
  // extern Chimera::Container::LightFlatMap<std::uintptr_t, size_t> InstanceToResourceIndex;
  // ... Additional Mappings

  /*------------------------------------------------
  Mappings from Chimera Config Options->Register Values
  ------------------------------------------------*/
  // extern const std::array<uint32_t, static_cast<size_t>( Chimera::GPIO::Pull::NUM_OPTIONS )> PullMap;
  // ... Additional Mappings

  /*-------------------------------------------------
  Module Functions
  -------------------------------------------------*/
  /**
   *  Initializes memory associated with mapping
   *
   *  @return void
   */
  void initializeMapping();
  
}}    // namespace Thor::LLD::{periph_uc}

#endif /* !{include_guard} */
"""

    @staticmethod
    def __module_template():
        return \
"""/********************************************************************************
 *  File Name:
 *    hw_{periph_lc}_mapping.cpp
 *
 *  Description:
 *    Provides structures for conversion and mapping between data types for fast
 *    runtime performance of driver code.
 *
 *  {year} | {author} | {email}
 ********************************************************************************/

/* STL Includes */
#include <array>

/* Driver Includes */
#include <Thor/cfg>
#include <Thor/lld/{family_lc}x/{periph_lc}/hw_{periph_lc}_mapping.hpp>

#if defined( TARGET_{family_uc} ) && defined( THOR_LLD_{periph_uc} )

namespace Thor::LLD::{periph_uc}
{{
  /*------------------------------------------------
  Chip Specific Resources
  ------------------------------------------------*/
  PeriphRegisterList PeripheralList;

  /*-------------------------------------------------
  Module Functions 
  -------------------------------------------------*/
  void initializeMapping()
  {{
    // PeripheralList[ GPIOA_RESOURCE_INDEX ] = GPIOA_PERIPH;
    // ... Add other peripherals
  }}

  /*-------------------------------------------------
  Initialize the Chimera Option to Register Config Mappings
  -------------------------------------------------*/
  /* clang-format off */
  // const std::array<uint32_t, static_cast<size_t>( Chimera::GPIO::Pull::NUM_OPTIONS )> PullMap = {{}};
  /* clang-format on */

}}    // namespace Thor::LLD::{periph_uc}

#endif /* TARGET_{family_uc} && THOR_LLD_{periph_uc} */
"""