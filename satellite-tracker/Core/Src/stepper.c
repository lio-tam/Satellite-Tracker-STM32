#include "stepper.h"

// Function to initialize the stepper motor
void stepper_init(StepperMotor *motor, GPIO_TypeDef *step_port, uint16_t step_pin,
                  GPIO_TypeDef *dir_port, uint16_t dir_pin, GPIO_TypeDef *button_port, uint16_t button_pin)
{
    // Initialize the motor's GPIO pins
    motor->step_port = step_port;
    motor->step_pin = step_pin;
    motor->dir_port = dir_port;
    motor->dir_pin = dir_pin;
    motor->button_port = button_port;
    motor->button_pin = button_pin;

    motor->steps_per_rotation = 0;  // Initialize the steps per rotation to 0
    motor->step_count = 0;          // Initialize step count to 0
    motor->is_clockwise = 0;        // Default direction is anticlockwise
}

// Function to toggle the motor's direction (clockwise/counterclockwise)
void toggle_motor_direction(StepperMotor *motor)
{
    //motor->is_clockwise = !motor->is_clockwise;  // Toggle direction flag
    if (motor->is_clockwise == 0)
    {
        HAL_GPIO_WritePin(motor->dir_port, motor->dir_pin, GPIO_PIN_SET);  // Set DIR pin to HIGH (clockwise)
        motor->is_clockwise = 1;
        HAL_Delay(1);  // Small delay
    }
    else
    {
        HAL_GPIO_WritePin(motor->dir_port, motor->dir_pin, GPIO_PIN_RESET);  // Set DIR pin to LOW (counterclockwise)
        motor->is_clockwise = 0;
        HAL_Delay(1);  // Small delay
    }
}

void set_motor_direction(StepperMotor *motor, int dir)
{
	if (motor->is_clockwise != dir)
	{
		toggle_motor_direction(motor);
	}

}

// Function to make the motor take a single step
void stepper_step(StepperMotor *motor)
{
    HAL_GPIO_TogglePin(motor->step_port, motor->step_pin);  // STEP pin HIGH (pulse)
    HAL_Delay(1);  // Step frequency delay

}

// Wait for the button press to indicate full rotation (calibration switch)
void wait_for_button_press(StepperMotor *motor)
{
    // Wait for the button (calibration switch) to be pressed (pin goes low when pressed)
    while (HAL_GPIO_ReadPin(motor->button_port, motor->button_pin) == GPIO_PIN_SET);  // Wait until the button is pressed
    HAL_Delay(50);  // Debounce delay

    // Wait for the button to be released before continuing
    while (HAL_GPIO_ReadPin(motor->button_port, motor->button_pin) == GPIO_PIN_RESET);  // Wait until button is released
    HAL_Delay(50);  // Debounce delay
}

// Stepper calibration function
void stepper_calibration(StepperMotor *motor)
{

    if (HAL_GPIO_ReadPin(motor->button_port, motor->button_pin) == GPIO_PIN_SET)
    {
    	for(int i=0; i<35; i++)
    	{
            stepper_step(motor);  // Take a step
    	}
    }

    toggle_motor_direction(motor);


    // Rotate the motor in the current direction (clockwise or counterclockwise)
    while (HAL_GPIO_ReadPin(motor->button_port, motor->button_pin) == GPIO_PIN_RESET)  // Wait for button press
    {
        stepper_step(motor);  // Take a step
        if (HAL_GPIO_ReadPin(motor->button_port, motor->button_pin) == GPIO_PIN_SET) break;
    }

    toggle_motor_direction(motor);
       HAL_Delay(5);  // Adjust this delay to control speed

       for(int i=0; i<18; i++)
           	{
                   stepper_step(motor);  // Take a step
           	}


    // Reset the step count after calibration
    motor->step_count = 0;


    while (HAL_GPIO_ReadPin(motor->button_port, motor->button_pin) == GPIO_PIN_RESET)  // Wait for button press (button is connected to GND, so it reads LOW when pressed)
               	    {
    					stepper_step(motor);
    				    motor->step_count++;  // Increment step count
    				 if (HAL_GPIO_ReadPin(motor->button_port, motor->button_pin) == GPIO_PIN_SET) break;
               	    }

    // After button press, store the step count as the steps per rotation
          motor->steps_per_rotation = motor->step_count;

    // Optionally toggle the direction for the next cycle
 	        HAL_Delay(5000);  // Adjust this delay to control speed




    // Wait for the button to be pressed again to indicate calibration is complete
   // wait_for_button_press(motor);
}

void stepper_rotate(StepperMotor *motor, int ang){
	if (ang > 0) set_motor_direction(motor, 0);
	else set_motor_direction(motor, 1);

    int steps_to_rotate = (int)(ang * (motor->steps_per_rotation / 348.0));
	for (int i=0; i< abs(steps_to_rotate);i++)
	{
		stepper_step(motor);
	}
}
