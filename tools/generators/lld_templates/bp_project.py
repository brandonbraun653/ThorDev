# **********************************************************************************************************************
#   FileName:
#       bp_project.py
#
#   Description:
#       Boilerplate generator for LLD project files
#
#   Usage Examples:
#       N/A
#
#   2020 | Brandon Braun | brandonbraun653@gmail.com
# **********************************************************************************************************************

from generators.lld_templates.bp_common import BaseAttributes


class LLDProjectTemplate(BaseAttributes):
    """ Generates starter header/source data for LLD prj files """

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
        return "THOR_HW_" + self.driver.upper() + "_PROJECT_HPP"

    def generate_header_filename(self):
        return "hw_" + self.driver.lower() + "_prj.hpp"

    def generate_header_text(self):
        template = self.__header_template()
        return template.format(periph_lc=self.driver.lower(), device_lc=self.device.lower(),
                               family_lc=self.family.lower(), chip_uc=self.chip.upper(),
                               year=self.year, author=self.author, email=self.email, include_guard=self.include_guard)

    def generate_source_filename(self):
        return None

    def generate_source_text(self):
        return None

    @staticmethod
    def __header_template():
        return \
"""/********************************************************************************
 *  File Name:
 *    hw_{periph_lc}_prj.hpp
 *
 *  Description:
 *    Pulls in target specific definitions and resources used in the actual driver
 *
 *  {year} | {author} | {email}
 ********************************************************************************/

#pragma once
#ifndef {include_guard}
#define {include_guard}

#if defined( STM32{chip_uc}xx )
#include <Thor/lld/{family_lc}x/{periph_lc}/variant/hw_{periph_lc}_register_{device_lc}.hpp>
#endif

#endif /* !{include_guard} */
"""
