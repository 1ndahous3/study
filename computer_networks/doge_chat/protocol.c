#include "protocol.h"

unsigned int crc32m(char *msg, size_t size) {
	unsigned int byte, crc, mask;
	crc = 0xFFFFFFFF;
	for (int i = 0; i < size && msg[i] != 0; i++) {
		byte = msg[i];
		crc ^= byte;
		for (int j = 0; j < 7; j++) {
			mask = -(crc & 1);
			crc = (crc >> 1) ^ (0xEDB88320 & mask);
		}
	}
	return ~crc;
}

meta_info meta_from_msg(char *msg) {
	meta_info pmsg;
	pmsg.length = strlen(msg) + 1;
	pmsg.hash = crc32m(msg, pmsg.length);
	return pmsg;
}

bool check_meta_info(meta_info *pmsg, char *msg) {
	return pmsg->length == strlen(msg) + 1 ?
			pmsg->hash == crc32m(msg, pmsg->length - 1) : false;
}

int send_string(int fd, char *msg) {
	int sent = 0;
	for (char *g = strtok(msg, "$"); g != NULL; g = strtok(NULL, "$")) {
		sent += send_massage(fd, msg);
	}
	return sent;
}

int send_massage(int fd, char *msg) {
	meta_info pmsg = meta_from_msg(msg);
	int sent = 0;
	for (size_t offset = 0; offset < sizeof(pmsg); offset += sent) {
		sent += send(fd, &pmsg + offset, sizeof(pmsg) - sent, 0);
	}
	sent = 0;
	for (size_t offset = 0; offset < pmsg.length - 1; offset += sent) {
		sent += send(fd, msg + offset, pmsg.length - 1 - sent, 0);
	}
	sent += send(fd, "$", 1, 0);
	return sent;
}

bool recv_massage(struct pollfd *pfd, char *msg, size_t size) {
	int recved;
	size_t offset = 0;

	while (pfd->revents == 0) {
		poll(pfd, 1, 100);
		while ((recved = recv(pfd->fd, (void *) msg + offset, size - offset,
				MSG_DONTWAIT)) > 0) {
			offset += recved;
			if (offset == size) {
				return true;
			}
		}
		if (recved == -1 && errno != EAGAIN) {
			pfd->revents |= POLLERR;
		}
		if (recved == 0) {
			pfd->revents |= POLLHUP;
		}
	}

	return false;
}

void *listener(void *arg) {
	meta_info pmsg;

    int id = *(int *)arg;

	struct pollfd pfd = { clients[id].fd, 0, 0 };

	while (pfd.revents == 0) {
		poll(&pfd, 1, 100);

		if (!recv_massage(&pfd, (char *) &pmsg, sizeof(meta_info))) {
			continue;
		}
		char *msg = (char *) alloca(pmsg.length);
		//printf("len = %lu\n", pmsg.length);
		if (!recv_massage(&pfd, msg, pmsg.length)) {
			continue;
		}
		msg[pmsg.length - 1] = '\0';
		//printf("%s\n", msg);

		if (check_meta_info(&pmsg, msg)) {
			message_handler(id, msg, &pfd.revents);
		} else {
			errproto_handler(id, &pfd.revents);
		}
	}

	print_dc_reason(pfd.revents);

	dc_handler(id);

	return NULL;
}

void print_dc_reason(short int revents) {
	if (revents & POLLERR) {
		printf("* An error has occurred on the stream.\n");
	}
	if (revents & POLLHUP) {
		printf("* Connection has been closed.\n");
	}
	if (revents & POLLNVAL) {
		printf("* The specified fd value is invalid.\n");
	}
}
