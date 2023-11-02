#include "main.h"
#include "lcd/lcd.h"
#include "lcd/lcd_graphics.h"

/** Design ideologies:
 * More small functions >>> Few giant functions
 * Reusability and Extendability is most important (Dont-Repeat-Yourself)
 * then Readability (comments are good, self documenting code is best)
 * No "magic numbers", use defines, constants, variables etc
 * ALWAYS INDENT CORRECTLY
 * No blocking (long while loops, HAL_Delay)
 */

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

ButtonState btn_state(void) {
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

/* Private variables START */

ButtonState state = ALL_OFF;

/* Private variables END */

/* Private function prototypes START */

/* Private function prototypes END */
volatile uint32_t last_change = 0;
bool flag = true;

void gpio_classwork(void) {
    /* Your code start here */
	ButtonState state_btn = btn_state();
	switch (state_btn)
	{
	case BTN1_ONLY:
		gpio_reset(LED1);
		break;
	case BTN2_ONLY:
		gpio_toggle(LED2);
		break;
	case BOTH_BTN:
		if (HAL_GetTick() - last_change > 1000)
		{
			last_change = HAL_GetTick();
			if (flag) flag = false; else flag = true;
		}
		if (flag){
			gpio_reset(LED1);
			gpio_reset(LED3);
			gpio_toggle(LED2);
		}
		else {
			gpio_toggle(LED1);
			gpio_toggle(LED3);
			gpio_reset(LED2);
		}
		break;
	case ALL_OFF:
		gpio_set(LED1);
		gpio_set(LED2);
		gpio_set(LED3);
		break;
	}
}
    /* Your code end here */
int32_t last_sec = 0;

void tft_classwork(void) {
    /* Your code start here */
		const int top = 50;
		int32_t time_elapse = HAL_GetTick();
      	int32_t minutes = time_elapse/60000;
      	int32_t seconds = time_elapse/1000 - minutes*60;
      	int32_t milli_secs = time_elapse - seconds*1000 - minutes*60000;
      	tft_prints(2, 2, "%02d:%02d:%02d", minutes, seconds, milli_secs);
      	if (seconds == 0) {
      		last_sec = 0;
      		flag = true;
      	}
      	if (seconds - last_sec >= 1){
      		last_sec = seconds;
      		if (flag) flag = false; else flag = true;
      	}
		if (flag){
			drawLine(16, top, 16, top+50, RED); //left
			drawLine(16, top+50, 66, top+50, RED); //bottom
			drawLine(16, top, 66, top, RED); //top
			drawLine(66, top, 66, top+50, RED); //right
		} else {
			drawLine(16, top, 16, top+50, GREEN); //left
			drawLine(16, top+50, 66, top+50, GREEN); //bottom
			drawLine(16, top, 66, top, GREEN); //top
			drawLine(66, top, 66, top+50, GREEN); //right
		}
    /* Your code end here */
}

uint32_t btn_press_time = 100000;
uint32_t btn_depress_time = 100000;

void tutorial2_homework(void) {
    /* Your code start here */
	// Level Targetting
	uint8_t state_btn1 = btn_read(BTN1);
	switch (state_btn1)
	{
	case 0:
		tft_prints(2, 2, "Hello, Haris");
		break;
	case 1:
		gpio_toggle(LED1);
		break;
	}

	// Edge Targetting
	ButtonState state_btn2 = btn_state();
	switch (state_btn2)
	{
		case BTN2_ONLY:
			if (flag) btn_press_time = HAL_GetTick();
			flag = false;
			break;
		default:
			if (!flag) btn_depress_time = HAL_GetTick();
			flag = true;
			break;
	}
	if (HAL_GetTick()-btn_press_time < 1000){
		tft_prints(2, 2, "Hello, Haris");
	}
	if (HAL_GetTick()-btn_depress_time < 1000){
		gpio_toggle(LED2);
	}

    /* Your code end here*/
}

// You can define your helper functions down below
// You can also modify the function definition above as long as your move is reasonable
