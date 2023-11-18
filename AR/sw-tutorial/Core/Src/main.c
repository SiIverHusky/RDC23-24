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
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd/lcd.h"
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
void gpio_classwork(void);
void tft_classwork(void);
void tutorial2_homework(void);
void pwm_init(void); //add this line!
void pwm_classwork(void); //add this line!
void pwm_homework(void); //add this line!
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int16_t last_error_frontL = 0;
        int16_t last_error_frontR = 0;
        int x = 0;
        int g = 1;
        static uint32_t last_ticks = 0;
        int temp = 0;
        int temp2 = 0;
		int temp3 = 0;
		int temp10 = 0;
		int coderun = 0;
		int beginover = 0;
		int var1 = 0;
		int var2 = 0;
		int var3 = 0;
		int var4 = 0;
		int var5 = 0;
		int var6 = 0;
		int var7 = 0;
		int var8 = 0;
		int var9 = 0;
		int var10 = 0;
		int var12 = 0;
		int var22 = 0;
		int firstpush = 0;
		int stopleft = 0;
		int finish = 0;
		int seasons  = 0;
		int leftCounter = 0;
		int firstlinedetector = 0;
		int box_check = 1;
		int releaseChecker = 0;
		int temp69 = 0;
		int finishtest = 0;
		int firstbox = 0;
		int secondbox = 0;
		int balls = 0;
		int ball1gone = 0;
		int temp11 = 0;
		int irRead = 0;
		int flag2 = 0;
		int flag3 = 0;
		static uint32_t delay_go_right = 0;

		int box[] = {1,2};


void right_wheel(int k){
		set_motor_speed(CAN1_MOTOR0, -k, 9, 0, 50, &last_error_frontL);
    }

void left_wheel(int k){
     	 set_motor_speed(CAN1_MOTOR2, k, 9, 0, 50, &last_error_frontR);
    }
int begin(){

	if (gpio_read(IR1) == 0 && gpio_read(IR2) == 0){

				left_wheel(100);
				right_wheel(100);
				tft_prints(0,1,"Front");
				return 1;

			}
			else if (gpio_read(IR1) == 1 && gpio_read(IR2) == 0) {
				left_wheel(-100);
				right_wheel(100);
				tft_prints(0,1,"right, left sensor");
				return 1;

			}//																IR1 is left, IR2 is right
			else if(gpio_read(IR1) == 0 && gpio_read(IR2) == 1){
				left_wheel(100);
				right_wheel(-100);
				tft_prints(0,1,"left, right sensor");
				return 1;

			}
			else if(gpio_read(IR1) == 1 && gpio_read(IR2) == 1){
				//left_wheel(0);
				//right_wheel(0);
				set_motor_current(CAN1_MOTOR0, 0);
				set_motor_current(CAN1_MOTOR2, 0);
				tft_prints(0,1,"STOP");
				return 2;


			}


}

int leftTouch(int k){

	if (var3 == 0){last_ticks = HAL_GetTick(); var3 = 1;}


	if (HAL_GetTick() - last_ticks > k){
			set_motor_current(CAN1_MOTOR0, 0);
			set_motor_current(CAN1_MOTOR2, 0);
			return 4;


				}
	else {

	left_wheel(-20);
	right_wheel(20);
	return 3;
	}

}

int pushAhead(int k){

	if (var1 == 0){last_ticks = HAL_GetTick(); var1 = 1;}

	if (HAL_GetTick() - last_ticks > k){
		set_motor_current(CAN1_MOTOR0, 0);
		set_motor_current(CAN1_MOTOR2, 0);
		return 3;


			}
	else {
				left_wheel(100);
				right_wheel(100);
				return 2;

			}

}

int pushAhead2(int k){

	if (var2 == 0){last_ticks = HAL_GetTick(); var2 = 1;}

	if (HAL_GetTick() - last_ticks > k){
		set_motor_current(CAN1_MOTOR0, 0);
		set_motor_current(CAN1_MOTOR2, 0);
		return 1;


			}
	else {
				left_wheel(100);
				right_wheel(100);
				return 0;

			}

}

int release(int ball){
	set_motor_current(CAN1_MOTOR0, 0);
	set_motor_current(CAN1_MOTOR2, 0);
	if (var4 == 0){last_ticks = HAL_GetTick(); var4 = 1;}

	if (ball == 1){
		gpio_set(pop2);
		if (HAL_GetTick() - last_ticks > 1000){
			gpio_reset(pop2);
			gpio_set(pop1);
			var4 = 0;
			return 1;

		}
	}
	else if (ball == 2){
		gpio_set(pop2);
		return 1;
	}


}
int FirstPart(){
	if (!btn_read(BTN1)){coderun = 1;}
				if (coderun == 1){
					if (finish == 0){

						finish = pushAhead2(2000);
						return 0;
					}


					if (finish == 1){
						//tft_prints(0, 6, "bbbbbbbbb");
						finish = begin();
						return 0;
					}


					if (finish == 2){
						//beginover = 2;

						//tft_prints(0, 6, "aaaaa");
						finish = pushAhead(1900);
						return 0;



					}
					if (finish == 3){
						// rotate left time

						finish = leftTouch(1700);
						return 0;

					}
					if (finish == 4){

						coderun = 2;
						return 1;

					}
					//tft_prints(0, 4, "wird");


				}



}

int box1(){

	if(gpio_read(IR1) == 1 && gpio_read(IR2) == 0){
		set_motor_current(CAN1_MOTOR0, 0);
		set_motor_current(CAN1_MOTOR2, 0);
		return 2;
		//releasechecker = release(1);

	}
	else{
		left_wheel(20);
		right_wheel(-20);
		return 1;
	}


}

void box2(){



	if(gpio_read(IR1) == 1 && gpio_read(IR2) == 0 && firstlinedetector == 0){
			set_motor_current(CAN1_MOTOR0, 0);
			set_motor_current(CAN1_MOTOR2, 0);
			firstlinedetector = 1;
			if (var5 == 0) {last_ticks = HAL_GetTick(); var5 = 1;}
			//releasechecker = release(1);

		}
		else if(firstlinedetector == 0)
		{
			left_wheel(20);
			right_wheel(-20);
		}
	if (firstlinedetector == 1){
		if (HAL_GetTick() - last_ticks < 300){
			left_wheel(20);
			right_wheel(-20);

	}

		else {

			if(gpio_read(IR1) == 1 && gpio_read(IR2) == 0){
				set_motor_current(CAN1_MOTOR0, 0);
				set_motor_current(CAN1_MOTOR2, 0);


			}
			else{

				left_wheel(20);
				right_wheel(-20);


			}



		}

}





}



int goright(){
//  if (var10 == 0) {delay_go_right = HAL_GetTick(); var10 = 1;}
//  if (HAL_GetTick() - delay_go_right > 500)
//  {
//	irRead = gpio_read(IR1);
//	delay_go_right = HAL_GetTick();
//  }
   if(gpio_read(IR1) == 1){
			set_motor_current(CAN1_MOTOR0, 0);
			set_motor_current(CAN1_MOTOR2, 0);
			tft_prints(0,3,"balls");
			tft_prints(0,1,"balls");
			tft_prints(0,2,"balls");
			tft_prints(0,4,"balls");
			tft_prints(0,5,"balls");
			tft_prints(0,6,"balls");

			//var9 = 0;
			return 1;


			//releasechecker = release(1);

		}
		else{

					left_wheel(20);
					right_wheel(-20);
					tft_prints(0,7,"please stop");
					return 0;

		}

}

int slightright(int k){

	if (var5 == 0){last_ticks = HAL_GetTick(); var5 = 1;}

	if (HAL_GetTick() - last_ticks < k){

		left_wheel(20);
		right_wheel(-20);


	}
	else {

		var5 = 0;
		return 1;

	}

}

int slightfront(int k){

	if (var7 == 0){last_ticks = HAL_GetTick(); var7 = 1;}

	if (HAL_GetTick() - last_ticks < k){

		left_wheel(20);
		right_wheel(20);


	}
	else {

		var7 = 0;

	}

}





void calculations(){

	int x = box[0];
	int y = box[1];

	firstbox = x;
	secondbox = y - x;




}

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
  MX_CAN1_Init();
  MX_CAN2_Init();
  //MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */

    // we turn off all the led first
    led_off(LED1);
    led_off(LED2);
    led_off(LED3);
    led_off(LED4);
    tft_init(PIN_ON_TOP, BLACK, WHITE, YELLOW, DARK_GREEN);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
    pwm_init();
    can_init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    tft_force_clear();


    // IR1(PC0), IR2(PC14), LIMIT1 (PC8)
    // IR1 is left sensor
    // IR2 is right sensor
    // Keep release 1 running for 1 second kinda depends on it

	gpio_set(pop1);
	//gpio_reset(pop2);
	int check = 0;
	int accbox = check;
	int check2 = 0;
	int accbox2 = check2;






//		if (HAL_GetTick() - last_ticks < 700){
//		test_pid(CAN2_MOTOR0, 20, 9, 0, 50, &last_error_frontR);
//		}
//		else if (HAL_GetTick() - last_ticks < 1400){
//		test_pid(CAN2_MOTOR0, -20, 9, 0, 50, &last_error_frontR);
//		}
//		else {
//			last_ticks = HAL_GetTick();
//		}


		// main code
//		if (temp10 == 1){
//			set_motor_current(CAN1_MOTOR0, 0);
//			set_motor_current(CAN1_MOTOR2, 0);
//		}

/* First Loop */

static char datBT[2];
bool btFlag = false;

while (1){
	can_ctrl_loop();
	tft_update(100);
	set_motor_current(CAN1_MOTOR0, 0);
	set_motor_current(CAN1_MOTOR2, 0);
	tft_prints(0, 0, "ON");
	if (!btFlag){
		tft_prints(0, 1, "BT Transfer...");
		datBT[0] = 'n';
		HAL_UART_Receive(&huart1, (uint8_t *)datBT, sizeof(datBT), 0xFFFF);
		if (datBT[0] != 'n')
		{
			tft_prints(0, 2, "%c%c", datBT[0], datBT[1]);
			check = datBT[0] - '0';
			check2 = datBT[1] - '0';
			btFlag = true;
			if (check > check2 && ((check < 1 && check > 4) || (check2 < 1 && check2 > 4)))
				btFlag = false;
		}
	}
	//seasons = 1;
	if (btFlag == true)
		break;

}

//// Testing BT output
//while(1)
//{
//	can_ctrl_loop();
//	tft_update(100);
//	set_motor_current(CAN1_MOTOR0, 0);
//	set_motor_current(CAN1_MOTOR2, 0);
//	tft_prints(0, 1, "%c %c", datBT[0], datBT[1]);
//	tft_prints(0, 2, "%d %d", check, check2);
//}
//
//
while (1){
	can_ctrl_loop();
	tft_update(100);
	seasons = FirstPart();
	if (seasons == 1){
		break;
	}
//
//}
//static char datBT[4];// bluetooth stuff
//int box_array[4] = {0, 0, 0, 0};

//while(1){// bluetooth stuff
//	bool check_bt = true;
//	datBT[0] = 'n';
//	can_ctrl_loop();
//	tft_update(100);
//	set_motor_current(CAN1_MOTOR0, 0);
//	set_motor_current(CAN1_MOTOR2, 0);
//	HAL_UART_Receive(&huart1, (uint8_t *)datBT, 4, 0xFFFF);
//	if (datBT[0] != 'n'){
//		for(int i = 0; i < 4; i++)
//		{
//			box_array[i] = datBT[i] - '0';
//		}
////		for (int i = 0; i < 4; i++)
////		{
////			if (box_array[i] < 0 || box_array[i] > 1)
////				check_bt = false;
////		}
//		if (check_bt)
//			break;
//	}
//
//
//}
//
//for (int i = 0; i < 4; i++){
//
//	if (box_array[i] == 1){
//
//		check2 = i + 1;
//		accbox = i + 1;
//
//	}
//
//
//
//
//}
//for (int i = 0; i < 3; i++){
//
//	if (box_array[i] == 1){
//
//		check = i + 1;
//		accbox = i + 1;
//		break;
//
//	}
//
//
//
//
//}


check2 = check2 - check;
//tft_update(100);
//
//tft_prints(0,1,"%c", datBT[0]);
//tft_prints(0,2,"%c", datBT[1]);
//tft_prints(0,3,"%c", datBT[2]);
//tft_prints(0,4,"%c", datBT[3]);
//
//
//tft_update(100);

// bluetooth stuff



//		calculations();
//
//
//		if (!btn_read(BTN1)){
//		seasons = 1;//FirstPart();
//		}
if (seasons == 1){

while(check != 0){

	while(gpio_read(IR1) == 0){
		can_ctrl_loop();
		tft_update(100);

		left_wheel(20);
		right_wheel(-20);



	}
	can_ctrl_loop();
	tft_update(100);

	set_motor_current(CAN1_MOTOR0, 0);
	set_motor_current(CAN1_MOTOR2, 0);
	seasons = 2;
	check--;
	if (check != 0){
		if (var5 == 0){last_ticks = HAL_GetTick(); var5 = 1;}
		while (HAL_GetTick() - last_ticks < 500)
		{
			can_ctrl_loop();
			tft_update(100);
			left_wheel(20);
			right_wheel(-20);

		}
		var5 = 0;
		can_ctrl_loop();
		tft_update(100);
		set_motor_current(CAN1_MOTOR0, 0);
		set_motor_current(CAN1_MOTOR2, 0);
		// move slightly right


	}



}


}

// pause for a second


last_ticks = HAL_GetTick();
while (HAL_GetTick() - last_ticks < 1000) {
			can_ctrl_loop();
			tft_update(100);
			set_motor_current(CAN1_MOTOR0, 0);
			set_motor_current(CAN1_MOTOR2, 0);

}


// if 1 need to go front
if (accbox == 1){
	can_ctrl_loop();
	tft_update(100);
	//move forward slightly
	if (var7 == 0){last_ticks = HAL_GetTick(); var7 = 1;}

		while (HAL_GetTick() - last_ticks < 1300){
			can_ctrl_loop();
			tft_update(100);
			left_wheel(10);
			right_wheel(10);
		}
	var7 = 0;
	can_ctrl_loop();
	tft_update(100);
	set_motor_current(CAN1_MOTOR0, 0);
	set_motor_current(CAN1_MOTOR2, 0);
	accbox = 10;
}




while(1){
	can_ctrl_loop();
	tft_update(100);

	if (var4 == 0){last_ticks = HAL_GetTick(); var4 = 1;}
			gpio_set(pop2);
			if (HAL_GetTick() - last_ticks > 1500){
				gpio_reset(pop2);
				gpio_reset(pop1);
				break;

			}


}// release the ball

if (accbox == 10){
	//move back slightly
	can_ctrl_loop();
	tft_update(100);
		//move forward slightly
		if (var7 == 0){last_ticks = HAL_GetTick(); var7 = 1;}

			while (HAL_GetTick() - last_ticks < 1300){
				can_ctrl_loop();
				tft_update(100);
				left_wheel(-10);
				right_wheel(-10);
			}
		var7 = 0;
		gpio_set(pop1);
		can_ctrl_loop();
		tft_update(100);
		set_motor_current(CAN1_MOTOR0, 0);
		set_motor_current(CAN1_MOTOR2, 0);


}

// if 1 need to go back




if (var12 == 0){last_ticks = HAL_GetTick(); var12 = 1;}
		while (HAL_GetTick() - last_ticks < 500)
		{
			can_ctrl_loop();
			tft_update(100);
			left_wheel(20);
			right_wheel(-20);

		}
		var12 = 0;
		can_ctrl_loop();
		tft_update(100);
		set_motor_current(CAN1_MOTOR0, 0);
		set_motor_current(CAN1_MOTOR2, 0);




while(check2 != 0){

	while(gpio_read(IR1) == 0){
		can_ctrl_loop();
		tft_update(100);

		left_wheel(20);
		right_wheel(-20);



	}
	can_ctrl_loop();
	tft_update(100);

	set_motor_current(CAN1_MOTOR0, 0);
	set_motor_current(CAN1_MOTOR2, 0);
	seasons = 2;
	check2--;
	if (check2 != 0){
		if (var5 == 0){last_ticks = HAL_GetTick(); var5 = 1;}
		while (HAL_GetTick() - last_ticks < 500)
		{
			can_ctrl_loop();
			tft_update(100);
			left_wheel(20);
			right_wheel(-20);

		}
		var5 = 0;
		can_ctrl_loop();
		tft_update(100);
		set_motor_current(CAN1_MOTOR0, 0);
		set_motor_current(CAN1_MOTOR2, 0);
		// move slightly right


	}



}

// SECOND DROP TIMEEEEEE
if (accbox2 == 4){
	can_ctrl_loop();
	tft_update(100);
	//move forward slightly
	if (var7 == 0){last_ticks = HAL_GetTick(); var7 = 1;}

		while (HAL_GetTick() - last_ticks < 3000){
			can_ctrl_loop();
			tft_update(100);
			left_wheel(20);
			right_wheel(25);
		}

	var7 = 0;
	can_ctrl_loop();
	tft_update(100);
	set_motor_current(CAN1_MOTOR0, 0);
	set_motor_current(CAN1_MOTOR2, 0);
	accbox = 10;
}




while(1){
	can_ctrl_loop();
	tft_update(100);

	if (var22 == 0){last_ticks = HAL_GetTick(); var22 = 1;}
			gpio_set(pop2);
			break;
			while (HAL_GetTick() - last_ticks < 3000){


			}
//			if (HAL_GetTick() - last_ticks > 1000){
//				gpio_reset(pop2);
//				gpio_set(pop1);
//				break;
//
//			}


}// release the ball

//if (accbox2 == 10){
//	//move back slightly
//	can_ctrl_loop();
//	tft_update(100);
//		//move forward slightly
//		if (var7 == 0){last_ticks = HAL_GetTick(); var7 = 1;}
//
//			while (HAL_GetTick() - last_ticks < 1500){
//				can_ctrl_loop();
//				tft_update(100);
//				left_wheel(-20);
//				right_wheel(-25);
//			}
//		can_ctrl_loop();
//		tft_update(100);
//		set_motor_current(CAN1_MOTOR0, 0);
//		set_motor_current(CAN1_MOTOR2, 0);
//
//
//}













while(1){
			can_ctrl_loop();
			tft_update(100);
			set_motor_current(CAN1_MOTOR0, 0);
			set_motor_current(CAN1_MOTOR2, 0);



}
//		if (seasons == 1){
//
//
//if (check != 0){
//	if (flag3 == 0){
//			if(gpio_read(IR1) == 1){
//						set_motor_current(CAN1_MOTOR0, 0);
//						set_motor_current(CAN1_MOTOR2, 0);
//						flag2 = 1;
//						flag3 = 1;
//
//
//						//releasechecker = release(1);
//
//					}
//			else {
//
//								left_wheel(20);
//								right_wheel(-20);
//
//					}
//
//	}
//			check--;
//			if (check != 0 && flag2 == 1){
//				if (var5 == 0){last_ticks = HAL_GetTick(); var5 = 1;}
//
//					if (HAL_GetTick() - last_ticks < 500){
//						left_wheel(20);
//						right_wheel(-20);
//
//
//					}
//					else {
//						set_motor_current(CAN1_MOTOR0, 0);
//						set_motor_current(CAN1_MOTOR2, 0);
//						var5 = 0;
//						flag3 = 0;
//						flag2 = 0;
//					}
//
//			}
//
//
//		}
//
//
//}








//		if (seasons == 1){
//
//
//				if (firstbox > 0){
//					//tft_prints(0,3,"firstbox is %d", firstbox);
//
//					if (temp10 == 0){
//
//						temp10 = goright();
//
//						//tft_prints(0,1,"temp10 is %d", temp10);
//
//					}
//					else if (temp10 == 1) {
//						set_motor_current(CAN1_MOTOR0, 0);
//						set_motor_current(CAN1_MOTOR2, 0);
//						firstbox = firstbox - 1;
//						temp10 = 2;
//						temp11 = 0;
//						tft_prints(0,0,"this should run");
//
//
//					}
//					else if (temp10 == 2 && balls == 0){
//
//						balls = slightright(150);
//						tft_prints(0,2,"balls is %d", balls);
//
//
//					}
//					else if (temp10 == 2 && balls == 1) {
//						set_motor_current(CAN1_MOTOR0, 0);
//						set_motor_current(CAN1_MOTOR2, 0);
//						balls = 0;
//						temp10 = 0;
//						tft_prints(0,4,"should not run");
//
//
//					}
//
//
//
//
//
//				}
//				else if (firstbox == 0 && ball1gone == 0){
//					set_motor_current(CAN1_MOTOR0, 0);
//					set_motor_current(CAN1_MOTOR2, 0);
//					tft_prints(0,0,"this should run");
//					ball1gone = release(1);
//
//
//				}
//				else {
//					set_motor_current(CAN1_MOTOR0, 0);
//					set_motor_current(CAN1_MOTOR2, 0);
//					tft_prints(0,6,"this should run");
//				}
//
//
//	}



		// First box (from the left)









//
//		static uint32_t last_ticks = 0;
//		if(HAL_GetTick()-last_ticks>100){
//			gpio_toggle(LED1);
//			last_ticks = HAL_GetTick();
//		}







		/*if (!btn_read(BTN1)){
			if (temp == 0){last_ticks = HAL_GetTick(); temp = 2;}
			release(1);
		//begin();
		}
		if (!btn_read(BTN2)){
			release(2);

		}*/



//set_motor_speed(CAN1_MOTOR0, 500, 9, 0, 50, &last_error_frontL);
//test_pid(CAN1_MOTOR2, 500, 9, 0, 50, &last_error_frontR);



// IR1(PC0), IR2(PC14), LIMIT1 (PC8)
//



// PC8 is IR3
// PC6 is IR2
// PC2 is pop1
// PC3 is pop2

//!btn_read(BTN1)

/*
		if (gpio_read(IR2) == 1){

			last_ticks = HAL_GetTick();

		}





		if (HAL_GetTick() - last_ticks < 1000){

			left_wheel(100);
			right_wheel(100);

		}
		else{

			left_wheel(0);
			right_wheel(0);
		}




		if (!btn_read(BTN1)) {
			//test_pid(CAN1_MOTOR0, 500, 9, 0, 50, &last_error_frontL);
			set_motor_speed(CAN1_MOTOR2, -500, 9, 0, 50, &last_error_frontR);

		    } else if (!btn_read(BTN2)) {
		    	//test_pid(CAN1_MOTOR0, -500, 9, 0, 50, &last_error_frontL);
		    	set_motor_speed(CAN1_MOTOR2, 500, 9, 0, 50, &last_error_frontR);
		    } else {
		    	//test_pid(CAN1_MOTOR0, 0, 9, 0, 50, &last_error_frontL);
		    	set_motor_speed(CAN1_MOTOR2, 0, 9, 0, 50, &last_error_frontR);
		    }
		//pb 14 IR 1
		if (gpio_read(IR1) == 0){
			tft_prints(0,1,"Hello");
		}
		else{
			tft_prints(0,1,"World");
		}*/




		/*if (!btn_read(BTN1)){

			last_ticks = HAL_GetTick();
		}


			if (HAL_GetTick() - last_ticks < 1000) {
						//last_ticks = HAL_GetTick();
					test_pid(CAN1_MOTOR0, 500, 9, 0, 50, &last_error_frontL);
					//set_motor_speed(CAN1_MOTOR0, 500, 9, 0, 50, &last_error_frontL);
					//test_pid(CAN1_MOTOR2, -500, 9, 0, 50, &last_error_frontR);
					set_motor_speed(CAN1_MOTOR2, -500, 9, 0, 50, &last_error_frontR);
					}
					else{
						set_motor_speed(CAN1_MOTOR0, 0, 9, 0, 50, &last_error_frontL);
								//test_pid(CAN1_MOTOR2, -500, 9, 0, 50, &last_error_frontR);
						set_motor_speed(CAN1_MOTOR2, 0, 9, 0, 50, &last_error_frontR);
					}*/







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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
    while (1) {
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
