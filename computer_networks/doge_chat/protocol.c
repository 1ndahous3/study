#include "protocol.h"

unsigned int crc32(char *msg, size_t size) {
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

prot_msg prot_from_msg(char *msg, size_t size) {
	prot_msg pmsg;
	pmsg.length = size;
	pmsg.hash = crc32(msg, size);
	strcpy(&pmsg.text, msg);
	return pmsg;
}

void msg_from_prot(char *msg, prot_msg *pmsg) {
	strcpy(msg, pmsg->text);
}

bool check_msg_info(prot_msg *pmsg) {
	return pmsg->length == strnlen(pmsg->text, MAX_MESSAGE_LENGTH) ?
			pmsg->hash == crc32(pmsg->text, pmsg->length) : false;
}

int send_massage(int fd, char *msg) {
	int sent = 0;
	char chunk[MAX_MESSAGE_LENGTH];
	size_t offset = 0;
	int size;
	for (;offset < strlen(msg);) {
		size = MIN(MAX_MESSAGE_LENGTH - 1, (strlen(msg) - offset) % (MAX_MESSAGE_LENGTH - 1)) == 0
				? (strlen(msg) - offset) % (MAX_MESSAGE_LENGTH - 1)
				: MAX_MESSAGE_LENGTH - 1;
		strncpy(chunk, msg + offset, size);
		offset += size;
		chunk[MAX_MESSAGE_LENGTH - 1] = '\0';
		sent += send_massage_chunk(fd, chunk);
	}
	return sent;
}

int send_massage_chunk(int fd, char *msg) {
	prot_msg pmsg = prot_from_msg(msg, strnlen(msg, MAX_MESSAGE_LENGTH));
	return send(fd, &pmsg, sizeof(prot_msg), 0);
}

void *listener(void *arg) {
	char msg[MAX_MESSAGE_LENGTH];
	prot_msg pmsg;

    int id = *(int *)arg;

	struct pollfd pfd = { clients[id].fd, 0, 0 };
	size_t offset = 0;
	int bytes_recv;

	while (pfd.revents == 0) {
		poll(&pfd, 1, 100);

		while ((bytes_recv = recv(pfd.fd, (void *) (&pmsg + offset), sizeof(prot_msg) - offset, MSG_DONTWAIT)) > 0) {
			offset += bytes_recv;
			if (offset == sizeof(prot_msg)) {
				if (check_msg_info(&pmsg)) {
					msg_from_prot(msg, &pmsg);
					message_handler(id, msg, &pfd.revents);
				} else {
					errproto_handler(id, &pfd.revents);
				}
				offset = 0;
				break;
			}
		}
		if (bytes_recv == -1 && errno != EAGAIN) {
			pfd.revents |= POLLERR;
		}
		if (bytes_recv == 0) {
			pfd.revents |= POLLHUP;
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
