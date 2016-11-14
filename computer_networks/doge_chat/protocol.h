#pragma once

#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX_MESSAGE_LENGTH 50
#define LOGIN_LENGTH 32

#pragma pack(push, 1)
typedef struct {
	size_t length;
	unsigned int hash;
	char text[MAX_MESSAGE_LENGTH];
} prot_msg;
#pragma pack(pop)

typedef struct {
	bool isAlive, isLogin;
	int fd;
	char nickName[LOGIN_LENGTH];
} client;

extern client clients[];

void message_handler(int id, char *msg, short int *revents);
void errproto_handler(int id, short int *revents);
void dc_handler(int id);

unsigned int crc32m(char *msg, size_t size);
void msg_from_prot(char *msg, prot_msg *pmsg);
prot_msg prot_from_msg(char *msg, size_t size);
bool check_msg_info(prot_msg *pmsg);

int send_massage(int fd, char *msg);
int send_massage_chunk(int fd, char *msg);

void print_dc_reason(short int revents);
void *listener(void *arg);
