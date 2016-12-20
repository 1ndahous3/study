#pragma once

#include "protocol.h"
#include <arpa/inet.h> //struct sockaddr_in, htons()
#include <unistd.h> //close()

#include <stdio.h>
#include <stdlib.h> //exit()
#include <stdbool.h>

#include <pthread.h>

#define MAX_CLIENTS 10

char *ip_addr;
int	port_num;
unsigned int active_users = 0;

pthread_mutex_t mutex;

enum recv_map {
	WELCOME,
	LIMIT,
	LOGIN_INCORRECT,
	LOGIN_EXIST,
	KICKED,
	ERR_PROTO
};

char *recv_msg[] = {
		"* Welcome to the chatroom",
		"* Oops, too much clients\n",
		"* Login is incorrect, try another\n",
		"* Login is already in use\n",
		"* You have been kicked by admin\n",
		"* Protocol error\n"
};

client clients[MAX_CLIENTS] = { 0 };

int main(int argc, char* argv[]);

void *server_session(void *arg);

void message_handler(int id, char **msg, uint32_t count, Type type, short int *revents) ;
void errproto_handler(int id, short int *revents);
void dc_handler(int id);

void broadcast(int id, char *msg);
void send_client_list(int id);

bool log_in(int id, char *msg);
bool check_login(char *msg);

void disconnect_user(client *client);
void kick_user(client *client, char *msg);
