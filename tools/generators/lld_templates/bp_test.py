# **********************************************************************************************************************
#   FileName:
#       bp_test.py
#
#   Description:
#       Boilerplate generator for LLD testing files
#
#   Usage Examples:
#       N/A
#
#   2020 | Brandon Braun | brandonbraun653@gmail.com
# **********************************************************************************************************************

from generators.lld_templates.bp_common import BaseAttributes


class LLDTestEntryTemplate(BaseAttributes):
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
        return "test_entry_lld_" + self.driver.lower() + ".cpp"

    def generate_source_text(self):
        template = self.__module_template()
        return template.format(periph_lc=self.driver.lower(), periph_uc=self.driver.upper(),
                               year=self.year, author=self.author, email=self.email,
                               include_guard=self.include_guard)

    @staticmethod
    def __module_template():
        return \
"""/********************************************************************************
 *  File Name:
 *    test_entry_lld_{periph_lc}.cpp
 *
 *	 Description:
 *    Entry into the test suite for LLD {periph_uc}
 *
 *  {year} | {author} | {email}
 *******************************************************************************/

#if defined( THOR_LLD_TEST_{periph_uc} )
#include "gtest/gtest.h"

int main( int argc, char **argv )
{{
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}}

#endif /* THOR_LLD_TEST_{periph_uc} */
"""


class LLDTestDriverTemplate(BaseAttributes):
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
        return "test_lld_" + self.driver.lower() + "_driver.cpp"

    def generate_source_text(self):
        template = self.__module_template()
        return template.format(periph_lc=self.driver.lower(), periph_uc=self.driver.upper(),
                               year=self.year, author=self.author, email=self.email,
                               include_guard=self.include_guard)

    @staticmethod
    def __module_template():
        return \
"""/********************************************************************************
 *  File Name:
 *    test_lld_{periph_lc}_driver.cpp
 *
 *	 Description:
 *    Tests the Thor {periph_uc} low level driver
 *
 *  {year} | {author} | {email}
 *******************************************************************************/

#if defined( THOR_LLD_TEST_{periph_uc} )
/* GTest Includes */
#include "gtest/gtest.h"

/* Chimera Includes */
#include <Chimera/common>
#include <Chimera/{periph_lc}>

/* Thor Includes */
#include <Thor/cfg>
#include <Thor/lld/interface/{periph_lc}/{periph_lc}.hpp>

TEST(Compiler, CanCompile)
{{
  EXPECT_EQ(0, 0);
  Thor::LLD::{periph_uc}::initialize();
}}

#endif  /* THOR_LLD_TEST_{periph_uc} */
"""