/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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

#include "can.h"
#include "dma.h"
#include "gpio.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd/lcd.h"
#include "movement.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint16_t stringlen(char *str) {
    int count = 0;

    while (*str != '\0') {
        count++;
        str++;
    }

    return count;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
//    MX_CAN1_Init();
    MX_CAN2_Init();
    MX_USART1_UART_Init();
    MX_DMA_Init();
    MX_USART2_UART_Init();
    MX_TIM5_Init();
    /* USER CODE BEGIN 2 */
    volatile uint32_t last_ticks = 0;

    // we turn off all the led first
    led_off(LED1);
    led_off(LED2);
    led_off(LED3);
    led_off(LED4);
    tft_init(PIN_ON_TOP, BLACK, WHITE, YELLOW, DARK_GREEN);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
    can_init();

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    tft_force_clear();

    double arr[5000];
    int i = 0;
    char temp[16];
    int j = 0;

    const int16_t Kp = 9;
    const int16_t Ki = 0;
    const int16_t Kd = 50;
    const int32_t Kp_a = 9000000;
    const int32_t Ki_a = 0;
    const int32_t Kd_a = 0;

    int16_t turn_up = 0;
    int16_t last_angle_up =0;

    int16_t turn_down = 0;
    int16_t last_angle_down =0;

    int16_t last_error_frontL = 0;
    int16_t last_error_frontR = 0;
    int16_t last_error_back = 0;

    int16_t last_error_up = 0;
    int16_t last_error_down = 0;
    double last_error_angle_up = 0;
    double last_error_angle_down = 0;

    // frontL CAN1_MOTOR2
    // frontR CAN1_MOTOR1
    // back CAN1_MOTOR0

    char moveVal = 's';

    while (1) {
        can_ctrl_loop();

        tft_update(100);
        if (HAL_GetTick() % 500 == 0) {
                   led_toggle(LED1);
               }
        // test pid
        if (HAL_GetTick() <= 5000) {
//        	set_motor_angle(CAN2_MOTOR1,70,Kp_a,Ki_a,Kd_a,turn_down, &last_error_angle_down, &last_error_down);
        } else if (HAL_GetTick()> 5000) {
//        	set_motor_angle(CAN2_MOTOR1,20,Kp_a,Ki_a,Kd_a,turn_down, &last_error_angle_down, &last_error_down);
        }/* else if (HAL_GetTick() > 2000 && HAL_GetTick() <= 3000) {
            test_pid(CAN1_MOTOR1, -500, Kp, Ki, Kd, &last_error);
        } else if (HAL_GetTick() > 3000) {
            test_pid(CAN1_MOTOR1, 0, Kp, Ki, Kd, &last_error);
        }/*

        tft_prints(0, 5, "TIME: %d", HAL_GetTick());

        if (i < 5000) {
            arr[i++] = get_motor_feedback(CAN1_MOTOR1).vel_rpm / 20.0;
        }

        if (i == 5000 && j < 5000) {
            snprintf(temp, 16, "%f", arr[j++]);
            temp[8] = '\n';
            temp[9] = '\0';
            HAL_UART_Transmit(&huart1, (uint8_t *)&temp, stringlen(temp), 1);
//        }*/
        check_turn(CAN2_MOTOR1 , &turn_down, &last_angle_down);
        set_motor_angle(CAN2_MOTOR1,70,Kp_a,Ki_a,Kd_a,turn_down, &last_error_angle_down, &last_error_down);
////
//        set_motor_speed(CAN2_MOTOR0,10,Kp,Ki,Kd,&last_angle_down);
        tft_prints(0,0,"%d  %d",get_motor_feedback(CAN2_MOTOR1).encoder,turn_down);

    }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {}
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
