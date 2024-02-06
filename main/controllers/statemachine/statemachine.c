#include <statemachine.h>
#include "TCP.h"
#include "config.h"

volatile State currentState;
volatile float joint_angles[6] = {90, 90, 90, 90, 90, 90};
void update_joint_angles(char *angles) {
    //6 angles separated by commas
    char *token = strtok(angles, ",");
    for (int i = 0; i < 6; i++) {
        joint_angles[i] = atof(token);
        token = strtok(NULL, ",");
    }
}

void functionInInitialState() {
    //set all join angles to 90
    update_joint_angles("90,90,90,90,90,90");
}

void setState(State newState) {
    currentState = newState;
}

void handleEvent(Event event) {
    switch (event) {
        case INITIAL_STATE_TO_TCP_READER_STATE:
            setState(TCP_READER_STATE);
            break;
        case TCP_READER_STATE_TO_INITIAL_STATE:
            setState(INITIAL_STATE);
            break;
    }
}

void stateMachineTask(void *pvParameters) {
    bool isPortActive = false;

    while (1) {
        switch (currentState) {
            case INITIAL_STATE:
                functionInInitialState();
                // Check if the desired port is active/used
                isPortActive = isConnected(SERVER_IP,SERVER_PORT);
                if (isPortActive) {
                    handleEvent(INITIAL_STATE_TO_TCP_READER_STATE);
                }
                break;
            case TCP_READER_STATE:
                // Ensure this task doesn't continuously spawn tcp_client_task
                if (!isPortActive) {
                    // Only switch to TCP_READER_STATE if not already in this state
                    read_from_tcp(SERVER_IP, SERVER_PORT);
                }
                break;
        }

        usleep(10000); // Sleep to prevent this loop from hogging CPU
    }
}
