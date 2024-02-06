#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
// Define states
typedef enum {
    INITIAL_STATE,
    TCP_READER_STATE,  
} State;

// Define events that cause state transitions
typedef enum {
    INITIAL_STATE_TO_TCP_READER_STATE,
    TCP_READER_STATE_TO_INITIAL_STATE,
} Event;

// Current state of the state machine
extern volatile State currentState;

extern volatile float joint_angles[6]; // Initialize all angles to 90

// Functions associated with states
void functionInInitialState();
void functionInTcpReaderState(); // This function should be defined elsewhere

// State machine control functions
void setState(State newState);
void handleEvent(Event event);
void stateMachineTask(void *pvParameters);


// Function to update joint angles from received data
void update_joint_angles(char *angles);


#endif // STATEMACHINE_H
