#pragma once

#include <arpa/inet.h> //struct sockaddr_in, htons()

#include "protocol.h"
#include "chatgui.h"

#include <pthread.h>

client clients[1];

int socket_fd;
pthread_t listen_thread;

int main(int argc, char* argv[]);
int init_connect(char* ip_addr, int port_num);
int send_message(int fd, char **msg, uint32_t count, Type type);

void message_handler(int id, uint8_t *buff, uint32_t length, short int *revents) ;
void errproto_handler(int id, short int *revents);
void dc_handler(int id);

#include "signals.h"
