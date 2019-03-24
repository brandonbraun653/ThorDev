#include <Thor/include/definitions.h>
#include <Thor/include/uart.h>

using namespace Thor::Definitions::Serial;
using namespace Thor::Peripheral::UART;

int main(void)
{
    /* Some test messages to send out */
    const char* test1 = "Hey mate.\r\n";
	const char* test2 = "Testing testing!\r\n";
	const char* test3 = "I love watermelon.\r\n";

    /* Create a brand new uart instance from UART4 */
    UARTClass_sPtr uart = UARTClass::create(4);
	uart->begin(SERIAL_BAUD_115200);

    /* Comment/Uncomment to switch modes */
	uart->setBlockMode(UARTPeriph::TX);
    //uart->setITMode(UARTPeriph::TX);
	//uart->setDMAMode(UARTPeriph::TX);

    /* If in IT or DMA mode, the text will be buffered */
    uart->write(test1);
    uart->write(test2);
    uart->write(test3);

    return 0;
}
