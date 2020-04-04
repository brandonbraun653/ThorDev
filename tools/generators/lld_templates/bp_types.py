# **********************************************************************************************************************
#   FileName:
#       bp_types.py
#
#   Description:
#       Boilerplate generator for LLD types files
#
#   Usage Examples:
#       N/A
#
#   2020 | Brandon Braun | brandonbraun653@gmail.com
# **********************************************************************************************************************

from generators.lld_templates.bp_common import BaseAttributes


class LLDTypesTemplate(BaseAttributes):
    """ Generates starter header/source data for LLD type files """

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    @property
    def has_header_file(self):
        return True

    @property
    def has_source_file(self):
        return False

    @property
    def include_guard(self):
        return "THOR_HW_" + self.driver.upper() + "_TYPES_HPP"

    def generate_header_filename(self):
        return "hw_" + self.driver.lower() + "_types.hpp"

    def generate_header_text(self):
        template = self.__header_template()
        return template.format(periph_lc=self.driver.lower(), periph_uc=self.driver.upper(),
                               family_uc=self.family.upper(), family_lc=self.family.lower(),
                               year=self.year, author=self.author, email=self.email,
                               include_guard=self.include_guard)

    def generate_source_filename(self):
        return None

    def generate_source_text(self):
        return None

    @staticmethod
    def __header_template():
        return \
"""/********************************************************************************
 *  File Name:
 *    hw_{periph_lc}_types.hpp
 *
 *  Description:
 *    {family_uc} Types for the {periph_uc} Peripheral
 *
 *  {year} | {author} | {email}
 ********************************************************************************/

#pragma once
#ifndef {include_guard}
#define {include_guard}

/* C++ Includes */
#include <cstdint>
#include <array>

/* Driver Includes */
#include <Thor/lld/{family_lc}x/{periph_lc}/hw_{periph_lc}_prj.hpp>

namespace Thor::LLD::{periph_uc}
{{
  struct RegisterMap
  {{
    /* Fill in registers here using "volatile Reg32_t <name>;" */
  }};

   using PeriphRegisterList = std::array<RegisterMap *, NUM_{periph_uc}_PERIPHS>;

}}    // namespace Thor::LLD::{periph_uc}

#endif /* !{include_guard}*/
"""
