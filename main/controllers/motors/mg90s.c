#include "mg90s.h"
#include "config.h"
esp_err_t ret;


// Initialize MCPWM for all servos
void mcpwm_initialize_all(void) {
    mcpwm_config_t pwm_config = {
        .frequency = 50,  // Frequency for servo motors (50Hz)
        .cmpr_a = 0,      // Duty cycle of PWMxA = 0
        .cmpr_b = 0,      // Duty cycle of PWMxb = 0
        .counter_mode = MCPWM_UP_COUNTER,
        .duty_mode = MCPWM_DUTY_MODE_0
    };

    // Initialize MCPWM for each servo GPIO
    esp_err_t ret;  // Reuse this variable for each initialization check
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, SERVO_0_GPIO);  // Servo 0
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, SERVO_1_GPIO);  // Servo 1
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2A, SERVO_2_GPIO);  // Servo 2
    mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0A, SERVO_3_GPIO);  // Servo 3, using timer 0's second output
    mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1B, SERVO_4_GPIO);  // Servo 4, using timer 1's second output

    // Apply the configuration to both timers of MCPWM unit 0
    ret = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
    ret = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);
    ret = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_2, &pwm_config);
    // Apply the configuration to both timers of MCPWM unit 1
    ret = mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_0, &pwm_config);
    ret = mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_1, &pwm_config);

    

    printf("MCPWM initialized for all servos\n");

}

// Set servo angle with consideration for servo 0 and 1 complementary movement
void set_servo_angle(mcpwm_unit_t unit, mcpwm_timer_t timer, float angle) {
    // Calculate pulse width for the given angle
    float pulse_width = SERVO_MIN_PULSEWIDTH + (SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * angle / 180.0f;

    mcpwm_set_duty_in_us(unit, timer, MCPWM_OPR_A, pulse_width);
}

