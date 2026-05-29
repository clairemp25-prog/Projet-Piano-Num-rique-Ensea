/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file : main.c
* @brief : Main program body
******************************************************************************
* @attention
*
* Copyright (c) 2026 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dac.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "karplus_strong.h"
#include "adsr.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// Structure d'une voix : KS + ADSR
typedef struct {
    KarplusStrong_t ks;
    ADSR_t env;
    uint8_t active;
} Voice_t;

// 9 voix = 9 touches
Voice_t voices[NUM_NOTES];

// Fréquences associées aux 9 touches

float noteFreq[NUM_NOTES] = {
    261.63f, // PB0  -> DO
    293.66f, // PB1  -> RE
    329.63f, // PB2  -> MI
    349.23f, // PB4  -> FA
    392.00f, // PB5  -> SOL
    440.00f, // PB11 -> LA
    493.88f, // PB13 -> SI
    523.25f, // PB14 -> DO+
    587.33f  // PB15 -> RE+
};

// Mapping GPIO
GPIO_TypeDef* notePort[NUM_NOTES] = {
    GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB
};

uint16_t notePin[NUM_NOTES] = {
    GPIO_PIN_0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_11,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15
};

void NoteOn(uint8_t id)
{
    KS_Init(&voices[id].ks, noteFreq[id], 0.996f);
    ADSR_NoteOn(&voices[id].env);
    voices[id].active = 1;
}

void NoteOff(uint8_t id)
{
    ADSR_NoteOff(&voices[id].env);
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_DAC1_Init();
  /* USER CODE BEGIN 2 */



  // Démarrer DAC + Timer audio
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_Base_Start_IT(&htim4);


    // Initialisation des voix
    for (int i = 0; i < NUM_NOTES; i++)
    {
        voices[i].active = 0;
        ADSR_Init(&voices[i].env,
                  0.002f,  // Attack très court (piano)
                  0.01f,   // Decay rapide
                  0.4f,    // Sustain faible
                  0.005f); // Release long
    }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // Lecture des 9 touches
	  for (int i = 0; i < NUM_NOTES; i++)
	      {
	          if (HAL_GPIO_ReadPin(notePort[i], notePin[i]) == GPIO_PIN_SET)
	          {
	              if (!voices[i].active)
	                  NoteOn(i);
	          }
	          else
	          {
	              if (voices[i].active)
	                  NoteOff(i);
	          }
	      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

	    static uint16_t val = 0;
	    val = (val == 0) ? 4095 : 0;
	    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, val);

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2)
	  {
	          float mix = 0.0f;

	          for (int i = 0; i < NUM_NOTES; i++)
	          {
	        	  if (voices[i].active)
	        	             {
	        	                 float sample = KS_Step(&voices[i].ks);
	        	                 float env    = ADSR_Step(&voices[i].env);

	        	                 mix += sample * env;

	        	                 if (voices[i].env.state == ADSR_OFF)
	        	                     voices[i].active = 0;
	        	             }
	        	         }

	        	         // Conversion float → DAC 12 bits
	        	         mix = (mix * 2047.0f) + 2048.0f;
	        	         if (mix < 0) mix = 0;
	        	         if (mix > 4095) mix = 4095;

	        	         HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t)mix);
	        	     }

  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
  void Error_Handler(void)
  {
    __disable_irq();
    while (1)
    {
    }
  }
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
