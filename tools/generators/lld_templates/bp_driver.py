# **********************************************************************************************************************
#   FileName:
#       bp_driver.py
#
#   Description:
#       Boilerplate generator for LLD driver files
#
#   Usage Examples:
#       N/A
#
#   2020 | Brandon Braun | brandonbraun653@gmail.com
# **********************************************************************************************************************

from generators.lld_templates.bp_common import BaseAttributes


class LLDDriverTemplate(BaseAttributes):
    """ Generates starter header/source data for LLD driver files """

    @property
    def has_header_file(self):
        return True

    @property
    def has_source_file(self):
        return True

    @property
    def include_guard(self):
        return "THOR_HW_" + self.driver.upper() + "_DRIVER_" + self.family.upper() + "_HPP"

    def generate_header_filename(self):
        return "hw_" + self.driver.lower() + "_driver.hpp"

    def generate_header_text(self):
        template = self.__header_template()
        return template.format(periph_lc=self.driver.lower(), periph_uc=self.driver.upper(),
                               family_uc=self.family.upper(), family_lc=self.family.lower(),
                               year=self.year, author=self.author, email=self.email,
                               include_guard=self.include_guard)

    def generate_source_filename(self):
        return "hw_" + self.driver.lower() + "_driver_" + self.family.lower() + ".cpp"

    def generate_source_text(self):
        template = self.__module_template()
        return template.format(periph_lc=self.driver.lower(), periph_uc=self.driver.upper(),
                               family_uc=self.family.upper(), family_lc=self.family.lower(),
                               year=self.year, author=self.author, email=self.email,
                               include_guard=self.include_guard)

    @staticmethod
    def __header_template():
        return \
"""/********************************************************************************
 *  File Name:
 *    hw_{periph_lc}_driver.hpp
 *
 *  Description:
 *    Declares the LLD interface to the {family_uc} series {periph_uc} hardware.
 *
 *  {year} | {author} | {email}
 ********************************************************************************/

#pragma once
#ifndef {include_guard}
#define {include_guard}

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/{periph_lc}>
#include <Chimera/thread>

/* Thor Includes */
#include <Thor/lld/{family_lc}x/{periph_lc}/hw_{periph_lc}_types.hpp>
#include <Thor/lld/{family_lc}x/{periph_lc}/hw_{periph_lc}_mapping.hpp>
#include <Thor/lld/interface/{periph_lc}/{periph_lc}_intf.hpp>
#include <Thor/lld/interface/interrupt/interrupt_model.hpp>

namespace Thor::LLD::{periph_uc}
{{
  class Driver : public IDriver
  {{
  public:
    Driver();
    ~Driver();

  private:
    RegisterMap *periph;
  }};
}}    // namespace Thor::LLD::{periph_uc}

#endif /* !{include_guard} */
"""

    @staticmethod
    def __module_template():
        return \
"""/********************************************************************************
 *  File Name:
 *    hw_{periph_lc}_driver_{family_uc}.cpp
 *
 *  Description:
 *    Implements the LLD interface to the {family_uc} series {periph_uc} hardware.
 *
 *  {year} | {author} | {email}
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/common>

/* Driver Includes */
#include <Thor/cfg>
//#include <Thor/lld/common/cortex-m4/utilities.hpp>
#include <Thor/lld/{family_lc}x/{periph_lc}/hw_{periph_lc}_driver.hpp>
#include <Thor/lld/{family_lc}x/{periph_lc}/hw_{periph_lc}_mapping.hpp>
#include <Thor/lld/{family_lc}x/{periph_lc}/hw_{periph_lc}_prj.hpp>
#include <Thor/lld/{family_lc}x/{periph_lc}/hw_{periph_lc}_types.hpp>
#include <Thor/lld/{family_lc}x/rcc/hw_rcc_driver.hpp>

#if defined( TARGET_{family_uc} ) && defined( THOR_LLD_{periph_uc} )

namespace Thor::LLD::{periph_uc}
{{
  static std::array<I{periph_uc}_sPtr, NUM_{periph_uc}_PERIPHS> s_{periph_lc}_drivers;

  /*-------------------------------------------------
  LLD->HLD Interface Implementation
  -------------------------------------------------*/
  Chimera::Status_t initialize()
  {{
    initializeRegisters();
    initializeMapping();

    return Chimera::CommonStatusCodes::OK;
  }}

  I{periph_uc}_sPtr getDriver( const size_t channel )
  {{
    if ( !( channel < NUM_{periph_uc}_PERIPHS ) )
    {{
      return nullptr;
    }}
    else if ( !s_{periph_lc}_drivers[ channel ] )
    {{
      s_{periph_lc}_drivers[ channel ] = std::make_shared<Driver>();
      s_{periph_lc}_drivers[ channel ]->attach( PeripheralList[ channel ] );
    }}

    return s_{periph_lc}_drivers[ channel ];
  }}

  size_t availableChannels()
  {{
    return NUM_{periph_uc}_PERIPHS;
  }}

  /*-------------------------------------------------
  Private LLD Function Implementation
  -------------------------------------------------*/
  bool is{periph_uc}( const std::uintptr_t address )
  {{
    bool result = false;

    for ( auto &val : periphAddressList )
    {{
      if ( val == address )
      {{
        result = true;
      }}
    }}

    return result;
  }}

  /*-----------------------------------------------------
  Low Level Driver Implementation
  -----------------------------------------------------*/
  Driver::Driver() : periph( nullptr )
  {{
  }}

  Driver::~Driver()
  {{
  }}

}}    // namespace Thor::LLD::{periph_uc}

#endif /* TARGET_{family_uc} && THOR_DRIVER_{periph_uc} */
"""