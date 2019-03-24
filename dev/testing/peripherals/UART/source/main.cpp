#include "test_uart.hpp"
#include "test_uart_settings.hpp"


#include <Thor/include/threads.hpp>
using namespace Thor::Threading;

TaskHandle_t ledThread;
TaskHandle_t rpcThread;
TaskHandle_t serThread;

int main(void)
{
	HAL_Init();
	ThorInit();

	addThread(ledTask, "led", 350, NULL, 1, &ledThread);
	addThread(serverTask, "rpcServer", 250, NULL, RPC_PRIORITY_LEVEL, &rpcThread);
	startScheduler();

	/* Should never reach here as scheduler should be running */
	for (;;)
	{

	}
}