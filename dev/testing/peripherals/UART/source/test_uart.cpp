/* Thor Includes */
#include <Thor/include/thor.hpp>
#include <Thor/include/nucleo.hpp>
#include <Thor/include/gpio.hpp>
#include <Thor/include/serial.hpp>
#include <Thor/include/threads.hpp>
#include <Thor/include/rpc.hpp>
#include <Thor/include/utilities.hpp>

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* eRPC Includes */
#include <Thor/erpc/setup/erpc_server_setup.h>
#include <Thor/erpc/transports/serial_thor_transport.h>

/* Auto-generated eRPC Includes */
#include "uart_rpc.h"
#include "uart_rpc_server.h"

/* Project Includes */
#include "test_uart.hpp"
#include "test_uart_settings.hpp"

using namespace Thor;
using namespace Thor::Nucleo;
using namespace Thor::Threading;
using namespace Thor::Peripheral::GPIO;
using namespace Thor::Peripheral::Serial;
using namespace Thor::Definitions::Serial;

SerialClass_sPtr serial;

GPIOClass_sPtr greenLed = boost::make_shared<GPIOClass>(GREEN_LED_PORT, GREEN_LED_PIN);
GPIOClass_sPtr blueLed = boost::make_shared<GPIOClass>(BLUE_LED_PORT, BLUE_LED_PIN);

/* Test Thread Handles */
TaskHandle_t hBlockingTX;

/*-------------------------------------------------------------------
*						   Test Implementations
*------------------------------------------------------------------*/
void ledTask(void* arguments)
{
	greenLed->mode(OUTPUT_PP);
	blueLed->mode(OUTPUT_PP);

	signalThreadSetupComplete();

	TickType_t lastTimeWoken = xTaskGetTickCount();
	for (;;)
	{
		blueLed->toggle();
		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(500));
	}
}

void serverTask(void* arguments)
{
	volatile erpc_status_t status;

	Thor::eRPC::erpcServerSetup(RPC_SERIAL_CHANNEL, create_TEST_UART_service);
	signalThreadSetupComplete();

	TickType_t lastTimeWoken = xTaskGetTickCount();
	for (;;)
	{
		// This thread only wakes up when new commands come over the serial port
		xSemaphoreTake(serverWakeup, portMAX_DELAY);
		status = erpc_server_poll();
	}
}

void setupSerialUnderTest()
{
	serial = boost::make_shared<SerialClass>(TEST_SERIAL_CHANNEL);
	serial->begin(SERIAL_BAUD_115200, BLOCKING, BLOCKING);
}


void test_blockingTX(void* arguments)
{
	serial->setMode(TX, BLOCKING);

	/* Various types of supported transmission data */
	uint8_t byte_data[7] = { 0x00, 0x10, 0x20, 0x30, 0x40, '\r', '\n' };
	char char_data[7] = { 'h', 'o', 'n', 'o', 'r', '\r', '\n' };
	const char* const_char_data = "Hey man\r\n";


	TickType_t lastTimeWoken = xTaskGetTickCount();
	for (;;)
	{
		//serial->write(byte_data, SIZE_OF_ARRAY(byte_data));
		//serial->write(char_data, SIZE_OF_ARRAY(char_data));
		serial->write("Hello World\r\n");
		//serial->write(const_char_data, strlen(const_char_data));

		vTaskDelayUntil(&lastTimeWoken, pdMS_TO_TICKS(1000));
	}
}

/*-------------------------------------------------------------------
 *						   eRPC FUNCTIONS
 *------------------------------------------------------------------*/
void turnGreenLEDON()
{
	greenLed->write(HIGH);
}

void turnGreenLEDOFF()
{
	greenLed->write(LOW);
}

void testStart_blockingTX()
{
	addThread(test_blockingTX, "blockTX", 350, NULL, TEST_PRIORITY_LEVEL, &hBlockingTX);
}

void testEnd_blockingTX()
{
	deleteThread(hBlockingTX);
}









