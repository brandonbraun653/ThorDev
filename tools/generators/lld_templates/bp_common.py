# **********************************************************************************************************************
#   FileName:
#       bp_common.py
#
#   Description:
#       Common functionality for all boilerplate generator types
#
#   Usage Examples:
#       N/A
#
#   2020 | Brandon Braun | brandonbraun653@gmail.com
# **********************************************************************************************************************

import logging
import re

from abc import abstractmethod
from pathlib import Path


class BaseAttributes(object):
    """ Common attributes that are needed in any file template """

    def __init__(self, *args, **kwargs):
        """
        :param device_name: The full chip name minus the variant letters (ie STM32F446xx, STM32L432xx)
        :type device_name: str

        :param driver_name: Name of the peripheral driver (SPI, GPIO, I2C, etc)
        :type driver_name: str

        :param year: The current year
        :type year: str

        :param author: Name of the person writing the module
        :type author: str

        :param email: Email of the author
        :type email: str
        """
        family_matcher = r"[a-zA-Z]{3}\d{2}\w{2}"
        chip_matcher = r"[a-zA-Z]\d{3}"
        variant_matcher = r"(\d{3})([a-zA-Z]{2})"

        logging.getLogger().setLevel(logging.INFO)

        self._driver_name = kwargs['driver']
        self._device_name = kwargs['device']
        self._family = re.search(family_matcher, kwargs['device']).group()
        self._chip = re.search(chip_matcher, kwargs['device']).group()
        self._variant = re.search(variant_matcher, kwargs['device']).group(2)
        self._year = kwargs['year']
        self._author = kwargs['author']
        self._email = kwargs['email']

    def write_to_file(self, root_directory, sub_dir=None):
        """
        Writes the associated header & module data to file.

        :param root_directory: Base directory from which the generator will reference
        :type root_directory: str or Path

        :param sub_dir: Optional subdirectory to place the generated files into
        :type sub_dir: str

        :return: None
        """
        # ---------------------------------------------
        # Make sure the expected directory structure exists
        # ---------------------------------------------
        root = Path(root_directory)
        if not root.exists():
            raise ValueError("{} is not a valid directory")

        root = root.joinpath(self.driver)
        root.mkdir(exist_ok=True)

        if sub_dir:
            root = root.joinpath(sub_dir)
            root.mkdir(exist_ok=True)

        # ---------------------------------------------
        # Write the header and source files
        # ---------------------------------------------
        if self.has_header_file:
            output_path = root.joinpath(self.generate_header_filename())
            with open(output_path, 'w') as f:
                logging.info("Writing [{}] header data to [{}]".format(self.driver, output_path))
                f.write(self.generate_header_text())

        if self.has_source_file:
            output_path = root.joinpath(self.generate_source_filename())
            with open(output_path, 'w') as f:
                logging.info("Writing [{}] source data to [{}]".format(self.driver, output_path))
                f.write(self.generate_source_text())

    @abstractmethod
    def generate_header_filename(self):
        """
        Generates the filename used save the header template text into. Note that this
        is not the fully qualified path, just the filename.

        :return: None or str
        """
        raise NotImplementedError("Method must be implemented in inheriting classes")

    @abstractmethod
    def generate_header_text(self):
        """
        Generates the text that should be placed in the header (.hpp) file. If the
        header file should not exist, returns None.

        :return: None or str
        """
        raise NotImplementedError("Method must be implemented in inheriting classes")

    @abstractmethod
    def generate_source_filename(self):
        """
        Generates the filename used save the module template text into. Note that this
        is not the fully qualified path, just the filename.

        :return: None or str
        """
        raise NotImplementedError("Method must be implemented in inheriting classes")

    @abstractmethod
    def generate_source_text(self):
        """
        Generates the text that should be placed in the module (.cpp) file. If the
        module file should not exist, returns None.

        :return: None or str
        """
        raise NotImplementedError("Method must be implemented in inheriting classes")

    @property
    def has_header_file(self):
        """
        Checks if the template generator has a header file
        :return: bool
        """
        raise NotImplementedError("Method must be implemented in inheriting classes")

    @property
    def has_source_file(self):
        """
        Checks if the template generator has a module file
        :return: bool
        """
        raise NotImplementedError("Method must be implemented in inheriting classes")

    @property
    def driver(self):
        return self._driver_name

    @property
    def device(self):
        return self._device_name

    @property
    def family(self):
        return self._family

    @property
    def short_family(self):
        """
        Strips off the 'STM32' characters from a family name to leave just
        the short abbreviation of the family name (ex STM32F4 -> F4).

        :return: str
        """
        temp = self._family.upper()
        return temp.replace("STM32", "")

    @property
    def chip(self):
        return self._chip

    @property
    def variant(self):
        return self._variant

    @property
    def year(self):
        return self._year

    @property
    def author(self):
        return self._author

    @property
    def email(self):
        return self._email

    @property
    @abstractmethod
    def include_guard(self):
        """
        Gets a string that is the <ig> portion of #ifdef <ig>
        :return: str
        """
        raise NotImplementedError("Method must be implemented in inheriting classes")
