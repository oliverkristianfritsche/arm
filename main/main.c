#include <stdio.h>
#include "mg90s.h"
#include <pthread.h>
#include "statemachine.h"
#include "TCP.h"
#include "config.h"
#include "wifi_manager.h"

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
    connect_to_wifi(WIFI_SSID, WIFI_PASSWORD);
    mcpwm_initialize_all();

    pthread_t servo_thread;
    pthread_t state_thread;
    //pthread_create(&servo_thread, NULL, servo_task, NULL);
    //pthread_create(&state_thread, NULL, stateMachineTask, NULL);
    
    

    addConnection(SERVER_IP, SERVER_PORT);
  

    pthread_t managerThread;
    pthread_create(&managerThread, NULL, connectionManager, NULL);

    pthread_join(managerThread, NULL); // In a real application, you might not join here.

    

}