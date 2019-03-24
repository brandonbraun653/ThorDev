#pragma once
#ifndef TEST_UART_SETTINGS_HPP
#define TEST_UART_SETTINGS_HPP

#define RPC_SERIAL_CHANNEL  7	/* Which serial channel to be used to communicate with computer */
#define RPC_PRIORITY_LEVEL	4	/* FreeRTOS priority level for RPC server */
#define TEST_PRIORITY_LEVEL 3	/* FreeRTOS priority level to be used for all test threads */
#define TEST_SERIAL_CHANNEL 5	/* Which serial channel to be used in the test module */



#endif /* !TEST_UART_SETTINGS_HPP */