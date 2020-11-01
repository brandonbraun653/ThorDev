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
import unittest


def execute_test_suite() -> None:
    """
    Executes the test suite

    This requires setting up the SocketCan interface properly using the below commands:
        sudo ip link set can0 type can bitrate 1000000
        sudo ifconfig can0 up

    :return: None
    """

    #can0 = can.interface.Bus(channel='can0', bustype='socketcan_ctypes')
    can0 = can.interface.Bus(bustype='slcan', channel='/dev/ttyACM0', bitrate=500000)
    msg = can.Message(arbitration_id=0x49, data=[0, 1, 2, 3, 4, 5, 6, 7], extended_id=False)

    try:
        can0.send(msg)
        print("Message sent on {}".format(can0.channel_info))
        can0.shutdown()
    except can.CanError as e:
        print(e)


if __name__ == "__main__":
    execute_test_suite()
