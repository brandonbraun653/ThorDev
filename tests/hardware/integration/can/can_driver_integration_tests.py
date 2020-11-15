# **********************************************************************************************************************
#   FileName:
#       can_driver_integration_tests.py
#
#   Description:
#       Driver integration tests for the Thor HLD
#
#   26 Oct 2020 | Brandon Braun | brandonbraun653@gmail.com
# **********************************************************************************************************************

import os
import can


def execute_test_suite() -> None:
    """
    Executes the test suite

    This requires setting up the SocketCan interface properly using the below commands:
        sudo ip link set can0 type can bitrate 500000
        sudo ifconfig can0 up

    :return: None
    """
    # os.system("sudo ip link set can0 type can bitrate 50000")
    # os.system("sudo ifconfig can0 up")

    can0 = can.interface.Bus(channel='can0', bustype='socketcan_ctypes')
    msg = can.Message(arbitration_id=0x49, data=[55, 66], extended_id=False)

    rx_msg = can0.recv(timeout=15)
    if rx_msg:
        print("Got RX message!")
    else:
        print("No RX message")
    # try:
    #     can0.send(msg)
    #     print("Message sent on {}".format(can0.channel_info))
    #     can0.shutdown()
    # except can.CanError as e:
    #     print(e)

    # os.system("sudo ifconfig can0 down")


if __name__ == "__main__":
    execute_test_suite()
