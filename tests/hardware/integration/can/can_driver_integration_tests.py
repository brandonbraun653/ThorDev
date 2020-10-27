# **********************************************************************************************************************
#   FileName:
#       can_driver_integration_tests.py
#
#   Description:
#       Driver integration tests for the Thor HLD
#
#   26 Oct 2020 | Brandon Braun | brandonbraun653@gmail.com
# **********************************************************************************************************************

import can
from can.interfaces.usb2can.serial_selector import serial


def execute_test_suite() -> None:
    """
    Executes the test suite
    :return:
    """
    serial_num = serial()
    bus = can.interface.Bus()
    msg = can.Message(arbitration_id=0, data=[0, 1, 2, 3, 4, 5], is_extended_id=False)

    try:
        bus.send(msg)
        print("Message sent on {}".format(bus.channel_info))
    except can.CanError:
        print("Message NOT sent")


if __name__ == "__main__":
    execute_test_suite()
