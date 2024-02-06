#ifndef TCP_H
#define TCP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>

// Maximum length for IPv4 address strings
#define IP_ADDRESS_LENGTH 16

#define MAX_CONNECTIONS 10
#define BUFFER_SIZE 1024  // Define the size of the read buffer


typedef struct {
    char ip[16];  // IPv4 address
    int port;
    int isConnected;  // 0: disconnected, 1: connected
    int sockfd;  // Socket file descriptor for the connection
    // Add other connection details as needed
} Connection;

extern volatile Connection connectionList[MAX_CONNECTIONS];
extern pthread_mutex_t connectionListMutex;

void* read_from_tcp(const char *ip, int port);
void* connectionManager(void *arg);
void addConnection(const char *ip, int port);
bool isConnected(const char *ip, int port);
#ifdef __cplusplus
}
#endif

#endif 
