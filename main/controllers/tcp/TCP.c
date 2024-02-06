#include "TCP.h"
#include "statemachine.h" // Include your state machine's header
#include "config.h"


volatile Connection connectionList[MAX_CONNECTIONS];
pthread_mutex_t connectionListMutex;

int connectionCount = 0;


void* read_from_tcp(const char *ip, int port) {
    
    pthread_mutex_lock(&connectionListMutex);
    for (int i = 0; i < connectionCount; i++) {
        if (strcmp(connectionList[i].ip, ip) == 0 && connectionList[i].port == port) {
            if (connectionList[i].isConnected) {
                char buffer[BUFFER_SIZE];
                memset(buffer, 0, BUFFER_SIZE);  // Clear the buffer

                ssize_t bytes_read = read(connectionList[i].sockfd, buffer, BUFFER_SIZE - 1);
                if (bytes_read > 0) {
                    // Successfully read data from the connection
                    printf("Data received from %s:%d - %s\n", ip, port, buffer);
                } else if (bytes_read == 0) {
                    // The connection was closed by the peer
                    printf("Connection closed by %s:%d\n", ip, port);
                    connectionList[i].isConnected = 0;  // Mark as disconnected
                    close(connectionList[i].sockfd);  // Close the socket
                } else {
                    // An error occurred during reading
                    perror("Read error");
                }
            } else {
                printf("Connection to %s:%d is not established.\n", ip, port);
            }
            pthread_mutex_unlock(&connectionListMutex);
            return NULL;  // Exit after handling the specified connection
        }
    }
    pthread_mutex_unlock(&connectionListMutex);
    sleep(1); // Wait a bit before retrying if the connection was not found or not connected
    
    return NULL;
}

void* connectionManager(void *arg) {

    pthread_mutex_init(&connectionListMutex, NULL);
    while (1) {
        pthread_mutex_lock(&connectionListMutex);
        for (int i = 0; i < connectionCount; i++) {
            if (connectionList[i].isConnected == 0) {
                // Attempt to connect
                int sock = socket(AF_INET, SOCK_STREAM, 0);
                struct sockaddr_in serverAddr;
                serverAddr.sin_family = AF_INET;
                serverAddr.sin_port = htons(connectionList[i].port);
                inet_pton(AF_INET, connectionList[i].ip, &serverAddr.sin_addr);

                if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == 0) {
                    printf("Connected to %s:%d\n", connectionList[i].ip, connectionList[i].port);
                    connectionList[i].isConnected = 1;
                    // Handle the connection (e.g., via another thread or callback)
                } else {
                    printf("Failed to connect to %s:%d\n", connectionList[i].ip, connectionList[i].port);
                    close(sock);
                }
            }
        }
        pthread_mutex_unlock(&connectionListMutex);
        pthread_mutex_destroy(&connectionListMutex);
        sleep(5); // Wait for 5 seconds before trying again
    }
    return NULL;
}

void addConnection(const char *ip, int port) {
    pthread_mutex_lock(&connectionListMutex);
    if (connectionCount < MAX_CONNECTIONS) {
        strncpy(connectionList[connectionCount].ip, ip, sizeof(connectionList[connectionCount].ip));
        connectionList[connectionCount].port = port;
        connectionList[connectionCount].isConnected = 0;
        connectionCount++;
    } else {
        printf("Maximum number of connections reached.\n");
    }
    pthread_mutex_unlock(&connectionListMutex);
}

bool isConnected(const char *ip, int port) {
    pthread_mutex_lock(&connectionListMutex);
    for (int i = 0; i < connectionCount; i++) {
        if (strcmp(connectionList[i].ip, ip) == 0 && connectionList[i].port == port) {
            pthread_mutex_unlock(&connectionListMutex);
            return connectionList[i].isConnected;
        }
    }
    pthread_mutex_unlock(&connectionListMutex);
    return false;
}
