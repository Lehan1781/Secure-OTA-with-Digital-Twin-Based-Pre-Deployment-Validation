#include "stm32f4xx_hal.h"
void SystemClock_Config(void);
void Error_Handler(void);

#define APP_ADDRESS  0x08008000

typedef void (*pFunction)(void);


static void jump_to_app(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    HAL_Delay(2000);  // Small delay so you can see bootloader run

    // Check if app is valid (stack pointer must be in RAM)
    if (((*(__IO uint32_t*)APP_ADDRESS) & 0x2FFE0000) == 0x20000000)
    {
        jump_to_app();
    }

    // If invalid app, stay in bootloader
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(200);
    }
}

static void jump_to_app(void)
{
    uint32_t app_stack = *(__IO uint32_t*)APP_ADDRESS;
    uint32_t app_reset = *(__IO uint32_t*)(APP_ADDRESS + 4);

    pFunction app_entry = (pFunction)app_reset;

    __disable_irq();

    // Deinit peripherals
    HAL_RCC_DeInit();
    HAL_DeInit();

    // Set MSP
    __set_MSP(app_stack);

    // Relocate vector table
    SCB->VTOR = APP_ADDRESS;

    // Jump
    app_entry();
}
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}