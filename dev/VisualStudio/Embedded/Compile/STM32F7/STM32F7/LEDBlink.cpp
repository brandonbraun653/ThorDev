#include <stm32f7/STM32F7xx_HAL_Driver/Inc/stm32f7xx_hal.h>

#ifdef __cplusplus
extern "C"
#endif
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

int main(void)
{
	HAL_Init();

	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Pin = GPIO_PIN_12;

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	for (;;)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_Delay(500);
	}
}
