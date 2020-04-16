# **********************************************************************************************************************
#   FileName:
#       bp_variant.py
#
#   Description:
#       Boilerplate generator for LLD variant files
#
#   Usage Examples:
#       N/A
#
#   2020 | Brandon Braun | brandonbraun653@gmail.com
# **********************************************************************************************************************

from generators.lld_templates.bp_common import BaseAttributes


class LLDVariantTemplate(BaseAttributes):
    """ Generates starter header/source data for LLD variant files """

    @property
    def has_header_file(self):
        return True

    @property
    def has_source_file(self):
        return True

    @property
    def include_guard(self):
        return "THOR_HW_" + self.driver.upper() + "_REGISTER_" + self.device.upper() + "_HPP"

    def generate_header_filename(self):
        return "hw_" + self.driver.lower() + "_register_" + self.device.lower() + ".hpp"

    def generate_header_text(self):
        template = self.__header_template()
        return template.format(periph_lc=self.driver.lower(), periph_uc=self.driver.upper(),
                               device_lc=self.device.lower(), device_uc=self.device.upper(),
                               family_uc=self.family.upper(), family_lc=self.family.lower(),
                               chip_uc=self.chip.upper(), chip_lc=self.chip.lower(),
                               year=self.year, author=self.author, email=self.email,
                               include_guard=self.include_guard)

    def generate_source_filename(self):
        return "hw_" + self.driver.lower() + "_register_" + self.device.lower() + ".cpp"

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
 *    hw_{periph_lc}_register_{device_lc}.hpp
 *
 *  Description:
 *    {periph_uc} register definitions for the {device_uc} series chips.
 *
 *  {year} | {author} | {email}
 ********************************************************************************/

#pragma once
#ifndef {include_guard}
#define {include_guard}

/* C++ Includes */
#include <array>
#include <cstdint>
#include <cstddef>

/* Driver Includes */
#include <Thor/lld/{family_lc}x/system/variant/sys_memory_map_{device_lc}.hpp>

/*-------------------------------------------------
Peripheral Availability
-------------------------------------------------*/
// #define STM32_{periph_uc}_PERIPH_AVAILABLE
// ...

namespace Thor::LLD::{periph_uc}
{{
  /**
   *  Initializes the LLD register resources and memory
   *  
   *  @return void
   */
  void initializeRegisters();

  /*-------------------------------------------------
  Peripheral Instance Memory Map Base
  -------------------------------------------------*/
  // static constexpr uint32_t {periph_uc}_BASE_ADDR = Thor::System::MemoryMap::{periph_uc}_PERIPH_START_ADDRESS;

  /*-------------------------------------------------
  Peripheral Resource Lookup Indices
  -------------------------------------------------*/
  // static constexpr uint32_t {periph_uc}_RESOURCE_INDEX = 0u;

  /*-------------------------------------------------
  Lookup addresses
  -------------------------------------------------*/
  // static constexpr size_t NUM_{periph_uc}_PERIPHS = 8;
  // static constexpr std::array<uint32_t, NUM_{periph_uc}_PERIPHS> periphAddressList = {{ ... }};

  /*-------------------------------------------------
  Peripheral Register Definitions
  -------------------------------------------------*/
  
}}    // namespace Thor::LLD::{periph_uc}

#endif /* !{include_guard} */"""

    @staticmethod
    def __module_template():
        return \
"""/********************************************************************************
 *  File Name:
 *    hw_{periph_lc}_register_{device_lc}.cpp
 *
 *  Description:
 *    {periph_uc} register definitions for the {device_uc} series chips.
 *
 *  {year} | {author} | {email}
 ********************************************************************************/

/* Driver Includes */
#include <Thor/cfg>
#include <Thor/lld/{family_lc}x/{periph_lc}/hw_{periph_lc}_driver.hpp>
#include <Thor/lld/{family_lc}x/{periph_lc}/hw_{periph_lc}_mapping.hpp>
#include <Thor/lld/{family_lc}x/{periph_lc}/variant/hw_{periph_lc}_register_{device_lc}.hpp>
#include <Thor/lld/{family_lc}x/{periph_lc}/hw_{periph_lc}_types.hpp>
#include <Thor/lld/{family_lc}x/rcc/hw_rcc_mapping.hpp>

#if defined( STM32{chip_uc}xx ) && defined( THOR_LLD_{periph_uc} )

namespace Thor::LLD::{periph_uc}
{{
#if defined( EMBEDDED )
  /*-------------------------------------------------
  Memory Mapped Structs to Peripherals
  -------------------------------------------------*/
  // RegisterMap *{periph_uc}_PERIPH = reinterpret_cast<RegisterMap *>( {periph_uc}_BASE_ADDR );

  /*-------------------------------------------------
  Lookup Tables Defintions
  -------------------------------------------------*/
  // Usually Chimera::LightFlatMap data types 

#elif defined( _SIM )
  /*-------------------------------------------------
  Memory Mapped Structs to Virtual Peripherals
  -------------------------------------------------*/
  // RegisterMap *{periph_uc}_PERIPH = nullptr;

  /*-------------------------------------------------
  Lookup Tables Definitions
  -------------------------------------------------*/
  // Chimera::Container::LightFlatMap<std::uintptr_t, size_t> InstanceToResourceIndex;
#endif

  void initializeRegisters()
  {{
#if defined( _SIM )
    /*------------------------------------------------
    Allocate some memory to simulate the register blocks
    ------------------------------------------------*/
    // {periph_uc}_PERIPH = new RegisterMap;

    /*------------------------------------------------
    Update the memory listing
    ------------------------------------------------*/
    // PeripheralList[ {periph_uc}_RESOURCE_INDEX ] = {periph_uc}_PERIPH;

    /*------------------------------------------------
    Update the resource indexer now that the registers actually exist
    ------------------------------------------------*/
    // InstanceToResourceIndex.append( reinterpret_cast<std::uintptr_t>( {periph_uc}_PERIPH ), {periph_uc}_RESOURCE_INDEX );

#endif
  }}
}}    // namespace Thor::LLD::{periph_uc}

namespace Thor::LLD::RCC::LookupTables
{{
  /*------------------------------------------------
  Lookup tables for register access on a peripheral by peripheral basis.
  Indexing must match the lookup table hw_{periph_lc}_mapping.hpp
  ------------------------------------------------*/
  RegisterConfig {periph_uc}_ClockConfig[ {periph_lc}TableSize ];
  RegisterConfig {periph_uc}_ClockConfigLP[ {periph_lc}TableSize ];
  RegisterConfig {periph_uc}_ResetConfig[ {periph_lc}TableSize ];
  Chimera::Clock::Bus {periph_uc}_SourceClock[ {periph_lc}TableSize ];

  const PCC {periph_uc}Lookup = {{
    {periph_uc}_ClockConfig, {periph_uc}_ClockConfigLP, {periph_uc}_ResetConfig, {periph_uc}_SourceClock, &Thor::LLD::{periph_uc}::InstanceToResourceIndex,
    {periph_lc}TableSize
  }};

  void {periph_uc}Init()
  {{
    using namespace Thor::LLD::{periph_uc};

    /*------------------------------------------------
    {periph_uc} clock enable register access lookup table
    ------------------------------------------------*/
    // {periph_uc}_ClockConfig[ {periph_uc}_RESOURCE_INDEX ].mask = AHB1ENR_{periph_uc}EN;
    // {periph_uc}_ClockConfig[ {periph_uc}_RESOURCE_INDEX ].reg  = &RCC1_PERIPH->AHB1ENR;

    /*------------------------------------------------
    {periph_uc} low power clock enable register access lookup table
    ------------------------------------------------*/
    // {periph_uc}_ClockConfigLP[ {periph_uc}_RESOURCE_INDEX ].mask = AHB1LPENR_{periph_uc}LPEN;
    // {periph_uc}_ClockConfigLP[ {periph_uc}_RESOURCE_INDEX ].reg  = &RCC1_PERIPH->AHB1LPENR;

    /*------------------------------------------------
    {periph_uc} reset register access lookup table
    ------------------------------------------------*/
    // {periph_uc}_ResetConfig[ {periph_uc}_RESOURCE_INDEX ].mask = AHB1RSTR_{periph_uc}RST;
    // {periph_uc}_ResetConfig[ {periph_uc}_RESOURCE_INDEX ].reg  = &RCC1_PERIPH->AHB1RSTR;

    /*------------------------------------------------
    {periph_uc} clocking bus source identifier
    ------------------------------------------------*/
    // {periph_uc}_SourceClock[ {periph_uc}_RESOURCE_INDEX ] = Chimera::Clock::Bus::PCLK1;
  }};

}}    // namespace Thor::LLD::RCC::LookupTables

#endif /* STM32{chip_uc}xx && THOR_LLD_{periph_uc} */
"""