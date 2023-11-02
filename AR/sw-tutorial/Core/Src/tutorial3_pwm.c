#include "main.h"
#include "tim.h"


/* Private variables END */

/* Private function prototypes START */

/* Private function prototypes END */

// Enums are always good
typedef enum {
    BOTH_BTN, BTN1_ONLY, BTN2_ONLY, ALL_OFF,
} ButtonState;

typedef enum {
    BUTTON1, BUTTON2,

    //This trick means the NumButtons will equal how many buttons there are (how many enum values)
    //This works as long as the enum values are simple (count up from 0)
    NUM_BUTTON,
} Button;

typedef enum {
    LED1, LED2, LED3, LED4, NUM_LED,
} LED;

/**
 * @brief read the button state
 * return 1 if the button is pressed;
 * return 0 if the button is released;
 */
static inline uint8_t read_button(Button btn) {
    switch (btn) {
    case BUTTON1:
        return !btn_read(BTN1);
    case BUTTON2:
        return !btn_read(BTN2);
    default:
        return 0;
    }
}

static ButtonState btn_state(void) {
    if (read_button(BUTTON1) && read_button(BUTTON2)) {
        return BOTH_BTN;
    } else if (read_button(BUTTON1)) {
        return BTN1_ONLY;
    } else if (read_button(BUTTON2)) {
        return BTN2_ONLY;
    } else {
        return ALL_OFF;
    }
}


void pwm_init(void) {
	// init the pwm prescaler value and auto-reload value and start the pwm
	/* Your code start here */
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
	TIM5 -> PSC = 99;
	TIM5 -> ARR = 16799;
	/* Your code end here */
}


volatile uint32_t pause = 0;
void pwm_classwork(void) {
	/* Your code start here */
	/*
	if (HAL_GetTick() - pause < 1000)
		TIM5 -> CCR1 = 419;			//Turn Clockwise
	else if (HAL_GetTick() - pause >= 1000 && HAL_GetTick() - pause < 2500)
	{
		TIM5 -> CCR1 = 1259; 		//In between()
	}
	else if (HAL_GetTick() - pause >= 2500 && HAL_GetTick() - pause < 3500)

		TIM5 -> CCR1 = 2099;		//Turn Anticlockwise
	else pause = HAL_GetTick();
	*/

	ButtonState controlMotor = btn_state();
	switch (controlMotor){
		case BTN1_ONLY :
			TIM5 -> CCR1 = 419;
			break;
		case BTN2_ONLY :
			TIM5 -> CCR1 = 2099;
			break;
		default:
			TIM5 -> CCR1 = 1259;
			break;
	}
	/* Your code end here */
}

volatile uint8_t counter = 0;
volatile uint8_t double_use = 0;
volatile uint8_t count_single = 0;
volatile uint8_t count_double = 0;
volatile uint8_t count_charge = 0;
volatile uint8_t count_special = 0;
uint8_t side = 0;
uint8_t flag_done = 0;
uint8_t charge = 0;
uint8_t flag_single = 0;
uint8_t flag_start_single = 0;
uint8_t flag_special = 0;
void pwm_homework(void) {
	/* Your code start here */
	if (count_single%2 == 0 && count_single != 0 && flag_single == 1)
	{
		flag_single = 0;
		double_use++;
	}
	if (flag_special == 1 && counter == 0)
	{
		flag_special = 0;
		count_special++;
		tft_prints(3,6, "SPECIAL MOVE");
		if (side == 0){
			TIM5 -> CCR1 = 2099;
			HAL_Delay(800);
			TIM5 -> CCR1 = 1899;
			HAL_Delay(800);
			TIM5 -> CCR1 = 2099;
			HAL_Delay(800);
			side = 1;
		} else if (side == 1){
			TIM5 -> CCR1 = 419;
			HAL_Delay(800);
			TIM5 -> CCR1 = 529;
			HAL_Delay(800);
			TIM5 -> CCR1 = 419;
			HAL_Delay(800);
			side = 0;
		}
		HAL_Delay(300);
		tft_prints(3,6, "");
	}
	ButtonState controlMotor = btn_state();
	switch (controlMotor) {
	case BTN1_ONLY:
		if (flag_start_single == 1){
			flag_start_single = 0;
			if (side == 0)
			{
				TIM5 -> CCR1 = 1259;
				HAL_Delay(600);
				count_single++;
				flag_single = 1;
				side = 1;
			} else if (side == 1)
			{
				TIM5 -> CCR1 = 1259;
				HAL_Delay(600);
				count_single++;
				flag_single = 1;
				side = 0;
			}
		}
		break;
	case BTN2_ONLY:
		if (double_use > 0)
		{
			flag_done = 0;
			if (side == 0)
			{
				TIM5 -> CCR1 = 839;
			}
			else if (side == 1)
			{
				TIM5 -> CCR1 = 1679;
			}
			counter++;
			if (charge == 0) charge = 1;
		}
		break;
	default:
		flag_start_single = 1;
		if (side == 0)
		{TIM5 -> CCR1 = 419;}
		else if (side == 1)
		{TIM5 -> CCR1 = 2099;}
		if (flag_done == 1) counter = 0;
		break;
	}
	if (flag_done == 0 && counter >= 20 && controlMotor == ALL_OFF && charge == 1){
		if (side == 0)
		{
			TIM5 -> CCR1 = 2099;
			HAL_Delay(1000);
			flag_done = 1;
			charge = 0;
			count_charge++;
			double_use--;
		}
		else if (side == 1){
			TIM5 -> CCR1 = 419;
			HAL_Delay(1000);
			flag_done = 1;
			charge = 0;
			count_charge++;
			double_use--;
		}
		if (count_charge %2 == 0 && count_charge >0)
		{
			flag_special = 1;
		}
	}
	else if (flag_done == 0 && counter < 20 && controlMotor == ALL_OFF && charge == 1){
		if (side == 0){
			TIM5 -> CCR1 = 2099;
			HAL_Delay(1000);
			flag_done = 1;
			charge = 0;
			count_double++;
			double_use--;
			side = 1;
		}
		else if (side == 1) {
			TIM5 -> CCR1 = 419;
			HAL_Delay(1000);
			flag_done = 1;
			charge = 0;
			count_double++;
			double_use--;
			side = 0;
		}
	}
	tft_prints(0,0, "Single Moves: %d", count_single);
	tft_prints(0,1, "Double Moves: %d", count_double);
	tft_prints(0,2, "Charged Moves: %d", count_charge);
	tft_prints(0,3, "Special Moves: %d", count_special);
	if (counter < 5)
	{
		gpio_set(LED1);
		gpio_set(LED2);
		gpio_set(LED3);
		gpio_set(LED4);
	} else if (counter >=5 && counter <10){
		gpio_reset(LED1);
		gpio_set(LED2);
		gpio_set(LED3);
		gpio_set(LED4);
	} else if (counter >= 10 && counter < 15){
		gpio_reset(LED1);
		gpio_reset(LED2);
		gpio_set(LED3);
		gpio_set(LED4);
	} else if (counter >= 15 && counter < 20)
	{
		gpio_reset(LED1);
		gpio_reset(LED2);
		gpio_reset(LED3);
		gpio_set(LED4);
	} else if (counter >= 20)
	{
		gpio_reset(LED1);
		gpio_reset(LED2);
		gpio_reset(LED3);
		gpio_reset(LED4);
	}
	/* Your code end here*/
}
