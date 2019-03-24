#pragma once
#ifndef TEST_UART_HPP
#define TEST_UART_HPP

/* Simple status led that blinks. Lets the user know if a crash occurred because the light stops blinking. */
extern void ledTask(void* arguments);

/* Handles polling the rpc server when new data arrives */
extern void serverTask(void* arguments);

#endif /* !TEST_UART_HPP */