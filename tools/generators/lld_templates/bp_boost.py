# **********************************************************************************************************************
#   FileName:
#       bp_boost.py
#
#   Description:
#       Boilerplate generator for LLD boost.build files
#
#   Usage Examples:
#       N/A
#
#   2020 | Brandon Braun | brandonbraun653@gmail.com
# **********************************************************************************************************************

from generators.lld_templates.bp_common import BaseAttributes


class LLDBuildSystemTemplate(BaseAttributes):
    """ Generates starter header/source data for LLD test entry files """

    @property
    def has_header_file(self):
        return False

    @property
    def has_source_file(self):
        return True

    @property
    def include_guard(self):
        return None

    def generate_header_filename(self):
        return None

    def generate_header_text(self):
        return None

    def generate_source_filename(self):
        return "build.jam"

    def generate_source_text(self):
        template = self.__module_template()
        return template.format(periph_lc=self.driver.lower(), periph_uc=self.driver.upper(),
                               family_lc=self.family.lower(), family_uc=self.family.upper(),
                               device_lc=self.device.lower(), device_uc=self.device.upper(),
                               chip_lc=self.chip.lower(), chip_uc=self.chip.upper(),
                               short_family_uc=self.short_family.upper())

    @staticmethod
    def __module_template():
        return \
"""# ====================================================
# Local Rules 
# ====================================================
local rule explicit_alias ( name : sources * : requirements * : default-build * : usage-requirements * )
    {{
    alias $(name) : $(sources) : $(requirements) : $(default-build) : $(usage-requirements) ;
    explicit $(name) ;
    }}

local path-constant dbg_install_dir = $(ARTIFACTS_DIR)/debug/lld/{family_lc}/{periph_lc} ;
local path-constant rel_install_dir = $(ARTIFACTS_DIR)/release/lld/{family_lc}/{periph_lc} ;

# ====================================================
# Public Targets
# ====================================================
explicit_alias PUB : : : : <define>THOR_LLD_{periph_uc} ;

# ====================================================
# Static Libraries
# ====================================================
lib lld_{periph_lc}_chip
  : variant/hw_{periph_lc}_register_{device_lc}.cpp

  : <Thor>enabled-{chip_uc}
    <link>static
    <use>/CHIMERA//PUB
    <use>/THOR//TARGET_PROPAGATED_OPTIONS
    <use>/THOR/LLD/{family_uc}//PUB
    <use>/THOR/LLD/{family_uc}/{periph_uc}//PUB
  ;
explicit lld_{periph_lc}_chip ;
explicit_alias DEVICE_TARGET : lld_{periph_lc}_chip : : : <library>lld_{periph_lc}_chip ;

lib lld_{periph_lc}
  : [ glob *.cpp ]

  : <link>static
    <use>/CHIMERA//PUB
    <use>/THOR//TARGET_PROPAGATED_OPTIONS
    <use>/THOR/LLD/{family_uc}//PUB
    <use>/THOR/LLD/{family_uc}/{periph_uc}//PUB
  ;
explicit lld_{periph_lc} ;
explicit_alias LIB : lld_{periph_lc} lld_{periph_lc}_chip : : : <library>lld_{periph_lc} <library>lld_{periph_lc}_chip ;

# ====================================================
# Test Driver Executable
# ====================================================
exe lld_{periph_lc}_test
  : [ glob test/*.cpp ]

    /THOR/LLD/{family_uc}//TEST_LIB
  
  : <link>static
    <Thor>enabled-{short_family_uc}
    <use>/AURORA//PUB
    <use>/CHIMERA//PUB
    <use>/GOOGLE//TEST_PUB
    <use>/THOR//PUB
    <use>/THOR//TARGET_PROPAGATED_OPTIONS
    <use>/THOR/LLD/{family_uc}//TEST_PUB
    <linkflags>-Wl,--no-as-needed
  ;
explicit lld_{periph_lc}_test ;
explicit_alias TEST : lld_{periph_lc}_test : : : <source>lld_{periph_lc}_test ;

# ====================================================
# Installation Targets
# ====================================================
install INSTALL_TEST : lld_{periph_lc}_test
  : <variant>release:<location>$(rel_install_dir)
    <variant>debug:<location>$(dbg_install_dir)
  ;

install INSTALL_LIB : lld_{periph_lc} lld_{periph_lc}_chip
  : <variant>release:<location>$(rel_install_dir)
    <variant>debug:<location>$(dbg_install_dir)
  ;

explicit_alias INSTALL_ALL : INSTALL_TEST INSTALL_LIB ;
"""


class LLDBuildGitIgnoreTemplate(BaseAttributes):
    """ Generates a gitignore filed for LLD build outputs"""

    @property
    def has_header_file(self):
        return False

    @property
    def has_source_file(self):
        return True

    @property
    def include_guard(self):
        return None

    def generate_header_filename(self):
        return None

    def generate_header_text(self):
        return None

    def generate_source_filename(self):
        return ".gitignore"

    def generate_source_text(self):
        return self.__module_template()

    @staticmethod
    def __module_template():
        return \
"""bin/*
**.a
**.exe
**.o"""
