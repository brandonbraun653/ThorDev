# **********************************************************************************************************************
#   FileName:
#       gen_lld_boilerplate.py
#
#   Description:
#
#
#   Usage Examples:
#       N/A
#
#   2020 | Brandon Braun | brandonbraun653@gmail.com
# **********************************************************************************************************************

from pathlib import Path
from argparse import ArgumentParser

from generators.lld_templates.bp_boost import LLDBuildSystemTemplate, LLDBuildGitIgnoreTemplate
from generators.lld_templates.bp_driver import LLDDriverTemplate
from generators.lld_templates.bp_mapping import LLDMappingTemplate
from generators.lld_templates.bp_project import LLDProjectTemplate
from generators.lld_templates.bp_test import LLDTestEntryTemplate, LLDTestDriverTemplate
from generators.lld_templates.bp_types import LLDTypesTemplate
from generators.lld_templates.bp_variant import LLDVariantTemplate


def main():
    parser = ArgumentParser()
    parser.add_argument("--chip", action="store", required=True,
                        help="Name of the chip being written for (ie STM32L432KC, STM32F446RE, etc")

    parser.add_argument("--driver", action="store", required=True,
                        help="Name of the driver module to create")

    parser.add_argument("--root", action="store", required=True,
                        help="Directory where the driver should be placed")

    # ---------------------------------------------
    # Parse and sanitize the inputs
    # ---------------------------------------------
    args, nargs = parser.parse_known_args()

    driver_name = args.driver

    root_dir = Path(args.root)
    if not root_dir.exists():
        raise ValueError("Specified root directory does not exist: {}".format(args['root']))

    year = "2020"
    author = "Brandon Braun"
    email = "brandonbraun653@gmail.com"

    # ---------------------------------------------
    # Generate the files needed for the low level driver boilerplate
    # ---------------------------------------------
    common_kwargs = {'driver': driver_name, 'device': args.chip, 'year': year, 'author': author, 'email': email}

    LLDBuildSystemTemplate(**common_kwargs).write_to_file(root_directory=root_dir)
    LLDBuildGitIgnoreTemplate(**common_kwargs).write_to_file(root_directory=root_dir)
    LLDDriverTemplate(**common_kwargs).write_to_file(root_directory=root_dir)
    LLDMappingTemplate(**common_kwargs).write_to_file(root_directory=root_dir)
    LLDProjectTemplate(**common_kwargs).write_to_file(root_directory=root_dir)
    LLDTestDriverTemplate(**common_kwargs).write_to_file(root_directory=root_dir, sub_dir='test')
    LLDTestEntryTemplate(**common_kwargs).write_to_file(root_directory=root_dir, sub_dir='test')
    LLDTypesTemplate(**common_kwargs).write_to_file(root_directory=root_dir)
    LLDVariantTemplate(**common_kwargs).write_to_file(root_directory=root_dir, sub_dir='variant')


if __name__ == '__main__':
    main()
