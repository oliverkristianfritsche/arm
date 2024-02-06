#include "TCP.h"
#include "state_machine.h" // Include your state machine's header



static int sock = -1;

// Function to handle TCP connection
static void tcp_client_task(void *pvParameters) {
    struct sockaddr_in serverAddr;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        vTaskDelete(NULL);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr.s_addr);

    while (1) {
        if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == 0) {
            printf("Connected to server\n");
            handleEvent(INITIAL_STATE_TO_TCP_READER_STATE); // Transition to TCP_READER_STATE

            // TCP connection established, can now receive data
            char rx_buffer[128];
            while (1) {
                int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
                if (len < 0) {
                    printf("Connection lost\n");
                    handleEvent(TCP_READER_STATE_TO_INITIAL_STATE); // Transition back to INITIAL_STATE
                    break;
                }
               update_joint_angles(rx_buffer); // Update joint angles from received data
                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                printf("Received: %s\n", rx_buffer);
            }
        } else {
            printf("Unable to connect to server. Retrying...\n");
            vTaskDelay(5000 / portTICK_PERIOD_MS); // Retry every 5 seconds
        }
    }

    if (sock != -1) {
        close(sock);
    }
    vTaskDelete(NULL);
}

void tcp_client_init(void) {
    xTaskCreate(tcp_client_task, "tcp_client_task", 4096, NULL, 5, NULL);
}
