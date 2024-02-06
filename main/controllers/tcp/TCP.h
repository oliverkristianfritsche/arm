#ifndef TCP_H
#define TCP_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

// Function to initialize TCP client
void tcp_client_init(void);

#endif // TCP_H
