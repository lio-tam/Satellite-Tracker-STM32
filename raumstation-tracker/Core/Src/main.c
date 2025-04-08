 /* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
////////////////////////////////////////////////////////////////////////////////
//Set Start coordinates:
double robotLat = 50.78;
double robotLon = 6.06;
////////////////////////////////////////////////////////////////////////////////

double robotAlt = 0.0;
volatile double satLat, satLon;
volatile double satAlt = 408000.0;
volatile bool dataready = 0;
/* USER CODE END PM */

double oldAzim = 0.0;
double oldElev = 0.0;

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
volatile uint8_t databuffer[35];
uint8_t rxbuffer[35];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
void parse_location(char* message);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define STEP_PIN1   GPIO_PIN_8
#define DIR_PIN1    GPIO_PIN_9
#define BUTTON_PIN1 GPIO_PIN_6

#define STEP_PORT1  GPIOB
#define DIR_PORT1   GPIOB
#define BUTTON_PORT1 GPIOC


// Define the GPIO pins for your STEP, DIR, and button (Motor2)
#define STEP_PIN2   GPIO_PIN_15
#define DIR_PIN2    GPIO_PIN_13
#define BUTTON_PIN2 GPIO_PIN_7

#define STEP_PORT2  GPIOB
#define DIR_PORT2   GPIOB
#define BUTTON_PORT2 GPIOC

// Stepper Motor instance
StepperMotor motor1;
StepperMotor motor2;
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
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  // Initialize the motor with specific GPIO pins for STEP, DIR, and button
   stepper_init(&motor1, STEP_PORT1, STEP_PIN1, DIR_PORT1, DIR_PIN1, BUTTON_PORT1, BUTTON_PIN1);
   stepper_init(&motor2, STEP_PORT2, STEP_PIN2, DIR_PORT2, DIR_PIN2, BUTTON_PORT2, BUTTON_PIN2);

   // Perform motor calibration to determine steps per rotation
   HAL_UART_Receive_IT(&huart3, rxbuffer, sizeof(rxbuffer));

   stepper_calibration(&motor1);
   stepper_calibration(&motor2);
//	  stepper_rotate(&motor1,  20);
//	  stepper_rotate(&motor2,  -20);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(dataready)
	  {
		  dataready = false;
		  parse_location(rxbuffer);
	  }

	  double newAzim = findAzimuth(satLat, satLon,  satAlt,  robotLat,  robotLon,  robotAlt);
	  double diffAzim = newAzim - oldAzim;
	  oldAzim = newAzim;
	  stepper_rotate(&motor2,  diffAzim);

	  double newElev = findElevation(satLat, satLon,  satAlt,  robotLat,  robotLon,  robotAlt);
	  double diffElev = newElev - oldElev;
	  oldElev = newElev;
	  stepper_rotate(&motor1,  diffElev);
	  memset(rxbuffer, 0, sizeof(rxbuffer));
	  HAL_UART_Receive_IT(&huart3, rxbuffer, sizeof(rxbuffer));  // Empfang erneut aktivieren

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {



		//memcpy(txbuffer, rxbuffer, sizeof(rxbuffer));

		  // Nullterminator hinzufügen

	    // Verarbeite die Nachricht bis zum '\n'
//	    char *newline_pos = strchr((char *)rxbuffer, '\n');
//	    if (newline_pos != NULL) {
//	        *newline_pos = '\0';  // Trenne den String an der Stelle von '\n'
//	    }
	    //strcat((char *)rxbuffer, "\r\n");
	   // HAL_UART_Transmit(&huart3, (uint8_t *)rxbuffer, strlen((char *)rxbuffer), 1);  // Daten senden


	   // memcpy (databuffer, rxbuffer, sizeof(databuffer));
	    rxbuffer[34] = '\0';
	    //memset(rxbuffer, 0, sizeof(rxbuffer));
	   // HAL_UART_Transmit(&huart3,databuffer ,sizeof(databuffer), 1);

	    dataready = true;









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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
	 GPIO_InitTypeDef GPIO_InitStruct = {0};
	 // Enable clocks for the necessary GPIO ports
	     __HAL_RCC_GPIOB_CLK_ENABLE(); // Enable clock for GPIOB (for STEP and DIR)
	     __HAL_RCC_GPIOC_CLK_ENABLE(); // Enable clock for GPIOC (for the BUTTON)
	     // Configure STEP (PB8) and DIR (PB9) pins as output
	         GPIO_InitStruct.Pin = STEP_PIN1 | DIR_PIN1 | STEP_PIN2 | DIR_PIN2;
	         GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Push-pull output
	         GPIO_InitStruct.Pull = GPIO_NOPULL;  // No internal pull-up or pull-down resistors
	         GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;  // Set low speed (for stepper motors, this might need adjusting)
	         HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  // Initialize GPIOB for STEP and DIR pins

/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
  // Configure BUTTON (PC6) pin as input with pull-up resistor
     GPIO_InitStruct.Pin = BUTTON_PIN1 | BUTTON_PIN2;
     GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  // Input mode
     GPIO_InitStruct.Pull = GPIO_PULLUP;  // Enable pull-up resistor for the button
     HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);  // Initialize GPIOC for the BUTTON pin
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */



void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {
        // Fehlerbehebung und Empfang neu starten
        HAL_UART_Receive_IT(&huart3, rxbuffer, sizeof(rxbuffer));
    }
}


void parse_location(char* message) {

    char *token;

    // Tokenize message (latitude and longitude are separated by ',')
    token = strtok(message, ",");
    if (token != NULL) {
    	satLat = atof(token); // Convert latitude to float
    }
    token = strtok(NULL, ",");
    if (token != NULL) {
        satLon = atof(token);
    }
    token = strtok(NULL, ",");
    if(token != NULL)
    {
       satAlt = atof(token);// Convert longitude to float
    }

    //memset(message, 0, sizeof(message));

}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
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

#ifdef  USE_FULL_ASSERT
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
