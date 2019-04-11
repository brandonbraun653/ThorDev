# **********************************************************************************
#   FileName:
#       run_cppcheck.py
#
#   Description:
#       Executes cppcheck on the Thor project files
#
#   Usage Examples:
#       N/A
#
#   2019 | Brandon Braun | brandonbraun653@gmail.com
# **********************************************************************************

import os
import sys

from tools.swqa.cppcheck import CppCheck

this_dir = os.path.dirname(__file__)

cppcheck = os.path.join(this_dir, 'tools', 'swqa', 'cppcheck', 'cppcheck.exe')
config_file = os.path.join(this_dir, 'thor_cppcheck.json')


def run_cpp_check():
    """
    Runs CppCheck on the Thor project

    :return: Process status code
    :rtype: int
    """
    checker = CppCheck(exe_path=cppcheck)
    checker.load_config(file=config_file)
    return checker.execute(config='all', working_dir=this_dir)


if __name__ == "__main__":
    sys.exit(run_cpp_check())
