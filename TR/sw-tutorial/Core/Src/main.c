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

char rx_buff[23]; //"sx+000Xy+000Yr+000Rb0B";

static int8_t x = 0;
static int8_t y = 0;
static int8_t r = 0;
static int8_t b = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    char storer[5];

    if (huart->Instance == huart1.Instance) {
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&rx_buff, 22);

        rx_buff[22] = '\0';

        tft_prints(0, 0, "%s", rx_buff);

        for (int i = 0; i < stringlen(rx_buff); i++) {
            switch (rx_buff[i]) {
                case 'x':
                    if (i + 4 <= 21) {
                        storer[0] = rx_buff[i + 1];
                        storer[1] = rx_buff[i + 2];
                        storer[2] = rx_buff[i + 3];
                        storer[3] = rx_buff[i + 4];
                        storer[4] = '\0';
                        if (abs(atoi(storer)) <= 100) {
                            x = atoi(storer);
                        }
                    } else if (i + 2 <= 21 || i + 3 <= 21) {
                        if (rx_buff[i + 2] == '0') {
                            x = 0;
                        } else {
                            storer[0] = rx_buff[i + 1];
                            storer[1] = '5';
                            storer[2] = '0';
                            storer[3] = '\0';
                            x = atoi(storer);
                        }
                    } else {
                        x = 0;
                    }
                    break;
                case 'X':
                    if (i - 4 >= 0) {
                        storer[0] = rx_buff[i - 4];
                        storer[1] = rx_buff[i - 3];
                        storer[2] = rx_buff[i - 2];
                        storer[3] = rx_buff[i - 1];
                        storer[4] = '\0';
                        if (abs(atoi(storer)) <= 100) {
                            x = atoi(storer);
                        }
                    } else if (i - 3 >= 0) {
                        if (rx_buff[i - 3] == '0') {
                            x = 0;
                        } else {
                            storer[0] = rx_buff[i - 3];
                            storer[1] = '5';
                            storer[2] = '0';
                            storer[3] = '\0';
                            x = atoi(storer);
                        }
                    } else {
                        x = 0;
                    }
                    break;
                case 'y':
                    if (i + 4 <= 21) {
                        storer[0] = rx_buff[i + 1];
                        storer[1] = rx_buff[i + 2];
                        storer[2] = rx_buff[i + 3];
                        storer[3] = rx_buff[i + 4];
                        storer[4] = '\0';
                        if (abs(atoi(storer)) <= 100) {
                            y = atoi(storer);
                        }
                    } else if (i + 2 <= 21 || i + 3 <= 21) {
                        if (rx_buff[i + 2] == '0') {
                            y = 0;
                        } else {
                            storer[0] = rx_buff[i + 1];
                            storer[1] = '5';
                            storer[2] = '0';
                            storer[3] = '\0';
                            y = atoi(storer);
                        }
                    } else {
                        y = 0;
                    }
                    break;
                case 'Y':
                    if (i - 4 >= 0) {
                        storer[0] = rx_buff[i - 4];
                        storer[1] = rx_buff[i - 3];
                        storer[2] = rx_buff[i - 2];
                        storer[3] = rx_buff[i - 1];
                        storer[4] = '\0';
                        if (abs(atoi(storer)) <= 100) {
                            y = atoi(storer);
                        }
                    } else if (i - 3 >= 0) {
                        if (rx_buff[i - 3] == '0') {
                            y = 0;
                        } else {
                            storer[0] = rx_buff[i - 3];
                            storer[1] = '5';
                            storer[2] = '0';
                            storer[3] = '\0';
                            y = atoi(storer);
                        }
                    } else {
                        y = 0;
                    }
                    break;
                case 'r':
                    if (i + 4 <= 21) {
                        storer[0] = rx_buff[i + 1];
                        storer[1] = rx_buff[i + 2];
                        storer[2] = rx_buff[i + 3];
                        storer[3] = rx_buff[i + 4];
                        storer[4] = '\0';
                        if (abs(atoi(storer)) <= 100) {
                            r = atoi(storer);
                        }
                    } else if (i + 2 <= 21 || i + 3 <= 21) {
                        if (rx_buff[i + 2] == '0') {
                            r = 0;
                        } else {
                            storer[0] = rx_buff[i + 1];
                            storer[1] = '5';
                            storer[2] = '0';
                            storer[3] = '\0';
                            r = atoi(storer);
                        }
                    } else {
                        r = 0;
                    }
                    break;
                case 'R':
                    if (i - 4 >= 0) {
                        storer[0] = rx_buff[i - 4];
                        storer[1] = rx_buff[i - 3];
                        storer[2] = rx_buff[i - 2];
                        storer[3] = rx_buff[i - 1];
                        storer[4] = '\0';
                        if (abs(atoi(storer)) <= 100) {
                            r = atoi(storer);
                        }
                    } else if (i - 3 >= 0) {
                        if (rx_buff[i - 3] == '0') {
                            r = 0;
                        } else {
                            storer[0] = rx_buff[i - 3];
                            storer[1] = '5';
                            storer[2] = '0';
                            storer[3] = '\0';
                            r = atoi(storer);
                        }
                    } else {
                        r = 0;
                    }
                    break;
                case 'b':
                    if (i + 1 <= 21) {
                        storer[0] = rx_buff[i + 1];
                        storer[1] = '\0';
                        if (atoi(storer) >= 0 && atoi(storer) <= 8) {
                            b = atoi(storer);
                        }
                    } else {
                        b = 0;
                    }
                    break;
                case 'B':
                    if (i - 1 >= 0) {
                        storer[0] = rx_buff[i - 1];
                        storer[1] = '\0';
                        if (atoi(storer) >= 0 && atoi(storer) <= 8) {
                            b = atoi(storer);
                        }
                    } else {
                        b = 0;
                    }
                    break;
                default: break;
            }
        }
    }
}

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
    MX_CAN1_Init();
    MX_CAN2_Init();
    MX_USART1_UART_Init();
    MX_DMA_Init();
    MX_USART2_UART_Init();
    MX_TIM5_Init();

    /* USER CODE BEGIN 2 */
    volatile uint32_t last_ticks = 0;

    HAL_UART_Receive_IT(&huart1, (uint8_t *)&rx_buff, 22);

    // we turn off all the led first
    led_off(LED1);
    led_off(LED2);
    led_off(LED3);
    led_off(LED4);
    tft_init(PIN_ON_TOP, BLACK, WHITE, YELLOW, DARK_GREEN);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
    gpio_reset(CYLINDER);
    can_init();

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    tft_force_clear();

    // test PID
    //    double arr[5000];
    //    int i = 0;
    //    char temp[16];
    //    int j = 0;

    const int16_t Kp = 9;
    const int16_t Ki = 0;
    const int16_t Kd = 50;

    int16_t last_error_frontL;
    int16_t last_error_frontR;
    int16_t last_error_back;

    // frontL  CAN2_MOTOR2
    // frontR  CAN2_MOTOR1
    // back    CAN2_MOTOR3
    // gripper CAN2_MOTOR0

    // control
    uint8_t gripper_mode = 0; // open is 0 close is 1
    int8_t current_gripper = 0;
    uint8_t auto_shortcut_mode = 0;
    uint32_t start_shortcut_tick = 0;

    int8_t speed_btn_pressed = 0;
    int8_t speed_mode = 0; // slow is 0 fast is 1
    int16_t speed = 75;    // motor rpm

    int8_t fan_btn_pressed = 0;
    int8_t fan_mode = 0; // stop is 0 open is 1
    int16_t last_error_fan;

    while (1) {
        can_ctrl_loop();

        tft_update(100);

        // DECODE BUTTONS
        switch (b) {
            case 1:
                if (gripper_mode == 0) {
                    gripper_mode = 1;
                    gpio_toggle(CYLINDER);
                    current_gripper = !current_gripper;
                }
                break;
            case 2:
                if (fan_btn_pressed == 0) {
                    fan_btn_pressed = 1;
                    fan_mode = !fan_mode;
                }
                break;
            case 3:
                if (auto_shortcut_mode == 0) {
                    auto_shortcut_mode = 1;
                };
                break;
            case 4:
                if (speed_btn_pressed == 0) {
                    speed_btn_pressed = 1;
                    speed_mode = !speed_mode;
                }
                break;
            case 5: r = -50; break;
            case 6: r = 50; break;
            case 0:
                gripper_mode = 0;
                speed_btn_pressed = 0;
                fan_btn_pressed = 0;
                break;
            default: break;
        }

        // CHANGE SPEED
        switch (speed_mode) {
            case 0: speed = 75; break;
            case 1: speed = 150; break;
            default: break;
        }

        // FAN
        switch (fan_mode) {
            case 1: set_motor_speed(CAN2_MOTOR0, 500, 9, 0, 50, &last_error_fan); break;
            case 0: set_motor_speed(CAN2_MOTOR0, 0, 9, 0, 50, &last_error_fan); break;
            default: break;
        }

        // MOVEMENT & AUTOSHORTCUT
        if (auto_shortcut_mode == 1) {
            start_shortcut_tick = HAL_GetTick();
            auto_shortcut_mode = 2;
        } else if (auto_shortcut_mode == 2 && HAL_GetTick() - start_shortcut_tick <= 3000) {
            move(CAN2_MOTOR2, CAN2_MOTOR1, CAN2_MOTOR3, -20, 100, 0, 150, &last_error_frontL, &last_error_frontR,
                 &last_error_back);
        } else if (auto_shortcut_mode == 2 && HAL_GetTick() - start_shortcut_tick >= 3000) {
            move(CAN2_MOTOR2, CAN2_MOTOR1, CAN2_MOTOR3, 0, 0, 0, speed, &last_error_frontL, &last_error_frontR,
                 &last_error_back);
            auto_shortcut_mode = 0;
        } else {
            move(CAN2_MOTOR2, CAN2_MOTOR1, CAN2_MOTOR3, x, y, r, speed, &last_error_frontL, &last_error_frontR,
                 &last_error_back);
        }

        tft_prints(0, 1, "x: %d ", x);
        tft_prints(0, 2, "y: %d ", y);
        tft_prints(0, 3, "r: %d ", r);
        tft_prints(0, 4, "b: %d", b);
        tft_prints(0, 8, "g: %d f: %d", current_gripper, fan_mode);
        tft_prints(0, 9, "speed: %d", speed);

        // test PID
        //        if (HAL_GetTick() <= 1000) {
        //            test_pid(CAN2_MOTOR3, 0, Kp, Ki, Kd, &last_error_frontL);
        //        } else if (HAL_GetTick() > 1000 && HAL_GetTick() <= 2000) {
        //            test_pid(CAN2_MOTOR3, 200, Kp, Ki, Kd, &last_error_frontL);
        //        } else if (HAL_GetTick() > 2000 && HAL_GetTick() <= 3000) {
        //            test_pid(CAN2_MOTOR3, 200, Kp, Ki, Kd, &last_error_frontL);
        //        } else if (HAL_GetTick() > 3000) {
        //            test_pid(CAN2_MOTOR1, 0, Kp, Ki, Kd, &last_error_frontL);
        //        }
        //
        //        tft_prints(0, 5, "TIME: %d", HAL_GetTick());
        //
        //        if (i < 5000) {
        //            arr[i++] = get_motor_feedback(CAN2_MOTOR1).vel_rpm / 20.0;
        //        }
        //
        //        if (i == 5000 && j < 5000) {
        //            snprintf(temp, 16, "%f", arr[j++]);
        //            temp[8] = '\n';
        //            temp[9] = '\0';
        //            HAL_UART_Transmit(&huart2, (uint8_t *)&temp, stringlen(temp), 1);
        //        }

        if (HAL_GetTick() == 0 && last_ticks != 0) {
            last_ticks = last_ticks % 1000;
        } else if (HAL_GetTick() - last_ticks >= 1000) {
            led_toggle(LED1);
            last_ticks = HAL_GetTick();
        }
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
