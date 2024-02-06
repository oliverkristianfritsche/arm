#include <stdio.h>
#include "mg90s.h"
#include <pthread.h>
#include "statemachine.h"
extern volatile float joint_angles[6] = {90}; // Initialize all angles to 90

void update_joint_angles(char *angles) {
    //6 singles separated by commas
    char *token = strtok(angles, ",");
    for (int i = 0; i < 6; i++) {
        joint_angles[i] = atof(token);
        token = strtok(NULL, ",");
    }
}

void *servo_task(void *vargp) {
    while (1) {
        for (int i = 0; i < 6; i++) {
            set_servo_angle(MCPWM_UNIT_0, i, joint_angles[i]);
        }
        usleep(10000); // 10ms delay
    }
}

void app_main(void)
{
    mcpwm_initialize_all();

    pthread_t servo_thread;
    pthread_t state_thread;
    pthread_create(&servo_thread, NULL, servo_task, NULL);
    pthread_create(&state_thread, NULL, stateMachineTask, NULL);


}