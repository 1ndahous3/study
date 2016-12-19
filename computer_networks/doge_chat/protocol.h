#pragma once

#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <alloca.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <endian.h>
#include "message.pb-c.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define MAX_MESSAGE_LENGTH 50
#define LOGIN_LENGTH 32

#pragma pack(push, 1)
typedef struct {
	uint32_t length;
	uint32_t hash;
} meta_info;
#pragma pack(pop)

typedef struct {
	bool isAlive, isLogin;
	int fd;
	char nickName[LOGIN_LENGTH];
} client;

extern client clients[];

void message_handler(int id, char **msg, uint32_t count, Type type, short int *revents);
void errproto_handler(int id, short int *revents);
void dc_handler(int id);

unsigned int crc32m(uint8_t *buff, size_t size);
meta_info meta_from_buffer(uint8_t *buff, size_t size);
bool check_buffer_meta(meta_info *meta, uint8_t *buff);

int send_buffer(int fd, char **msg, uint32_t count, Type type);
bool recv_message(struct pollfd *pfd, uint8_t *msg, size_t size);

void print_dc_reason(short int revents);
void *listener(void *arg);
