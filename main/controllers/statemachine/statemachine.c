#include <stdio.h>
#include <stdbool.h>
#include <statemachine.h>

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
    while (1) {
        switch (currentState) {
            case INITIAL_STATE:
                functionInInitialState();
                break;
            case TCP_READER_STATE:
                functionInTcpReaderState();
                break;
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
