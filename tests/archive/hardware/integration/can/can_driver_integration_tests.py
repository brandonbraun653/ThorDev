# **********************************************************************************************************************
#   FileName:
#       can_driver_integration_tests.py
#
#   Description:
#       Driver integration tests for the Thor HLD
#
#   26 Oct 2020 | Brandon Braun | brandonbraun653@gmail.com
# **********************************************************************************************************************

import time
import can
import unittest


class TestSendReceive(unittest.TestCase):

    def test_transactions(self):
        """
        Executes the test suite

        This requires setting up the SocketCan interface properly using the below commands:
            sudo ip link set can0 type can bitrate 500000
            sudo ifconfig can0 up

        Then when testing is complete, tear down the interface using:
            sudo ifconfig can0 down

        :return: None
        """
        # ---------------------------------------------
        # Test Data
        # ---------------------------------------------
        rx_messages = [
            can.Message(arbitration_id=0x50, data=[0x33], is_extended_id=False),
            can.Message(arbitration_id=0x43f550, data=[0xAA], is_extended_id=True)
        ]

        tx_messages = [
            can.Message(arbitration_id=0x50, data=[0x33], is_extended_id=False),
            can.Message(arbitration_id=0x43f550, data=[0xAA], is_extended_id=True)
        ]

        acceptance_filters = [
            {"can_id": 0x22, "can_mask": 0x22, "extended": False},  # Sync frame
            {"can_id": 0x50, "can_mask": 0x50, "extended": False},  # Start of Test Frame
            {"can_id": 0x43f550, "can_mask": 0x43f550, "extended": True}
        ]

        expected_ids = [fltr["can_id"] for fltr in acceptance_filters]

        # ---------------------------------------------
        # Initialize the CAN interface
        # ---------------------------------------------
        can0 = can.interface.Bus(channel='can0', bustype='socketcan')
        can0.set_filters(filters=acceptance_filters)
        can0.flush_tx_buffer()

        # ---------------------------------------------
        # Wait to receive the sync frame to know when to start
        # ---------------------------------------------
        sync_frame = can.Message(arbitration_id=0x22, data=[0xFF], is_extended_id=False)
        while True:
            tmp = can0.recv()
            if (tmp.arbitration_id == sync_frame.arbitration_id) and (tmp.data == sync_frame.data):
                break
            time.sleep(0.1)

        # ---------------------------------------------
        # Iterate over the test steps
        # ---------------------------------------------
        for idx in range(len(rx_messages)):
            # ---------------------------------------------
            # Wait to receive a message. We still seem to
            # get garbage IDs though, so filter them out.
            # ---------------------------------------------
            rx_msg = can0.recv(timeout=15)
            self.assertIsNotNone(rx_msg, "Failed to receive a message from the DUT")

            while rx_msg.arbitration_id not in expected_ids:
                rx_msg = can0.recv(timeout=15)
                self.assertIsNotNone(rx_msg, "Failed to receive a message from the DUT")

            # ---------------------------------------------
            # Check message that made it past filtering
            # ---------------------------------------------
            self.assertEqual(rx_msg.arbitration_id, rx_messages[idx].arbitration_id)
            self.assertEqual(rx_msg.is_extended_id, rx_messages[idx].is_extended_id)
            self.assertEqual(rx_msg.data, rx_messages[idx].data)
            time.sleep(0.5)

            # ---------------------------------------------
            # Send the expected response
            # ---------------------------------------------
            can0.send(tx_messages[idx])
            time.sleep(0.5)

        # ---------------------------------------------
        # Destroy the test
        # ---------------------------------------------
        can0.shutdown()


if __name__ == "__main__":
    unittest.main()
