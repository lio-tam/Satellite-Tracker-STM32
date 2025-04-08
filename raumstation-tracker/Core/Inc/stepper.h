#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include "stm32f4xx_hal.h"

// Stepper Motor Structure
typedef struct {
    GPIO_TypeDef *step_port;   // GPIO port for STEP pin
    uint16_t step_pin;         // GPIO pin for STEP
    GPIO_TypeDef *dir_port;    // GPIO port for DIR pin
    uint16_t dir_pin;          // GPIO pin for DIR
    GPIO_TypeDef *button_port; // GPIO port for button (calibration switch)
    uint16_t button_pin;       // GPIO pin for button (calibration switch)
    int steps_per_rotation;    // Variable to store steps per rotation
    int step_count;            // Count the steps taken
    int is_clockwise;          // Direction flag (1 = clockwise, 0 = counterclockwise)
} StepperMotor;

// Function Declarations

// Initializes the stepper motor
void stepper_init(StepperMotor *motor, GPIO_TypeDef *step_port, uint16_t step_pin,
                  GPIO_TypeDef *dir_port, uint16_t dir_pin, GPIO_TypeDef *button_port, uint16_t button_pin);

// Makes the motor take a single step
void stepper_step(StepperMotor *motor);

// Performs calibration to determine the steps per rotation
void stepper_calibration(StepperMotor *motor);

// Toggles the motor's rotation direction (clockwise/counterclockwise)
void toggle_motor_direction(StepperMotor *motor);

// Set motot direction to dir (clockwise =1/counterclockwise =0)
void set_motor_direction(StepperMotor *motor, int dir);

// Rotate the stepper with certain ang
void stepper_rotate(StepperMotor *motor, int ang);

// Waits for the button press to indicate full rotation (calibration switch)
void wait_for_button_press(StepperMotor *motor);

#endif /* STEPPER_MOTOR_H */
