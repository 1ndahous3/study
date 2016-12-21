#include "protocol.h"

unsigned int crc32m(uint8_t *buff, size_t size) {
	unsigned int byte, crc, mask;
	crc = 0xFFFFFFFF;
	for (int i = 0; i < size; i++) {
		byte = buff[i];
		crc ^= byte;
		for (int j = 0; j < 7; j++) {
			mask = -(crc & 1);
			crc = (crc >> 1) ^ (0xEDB88320 & mask);
		}
	}
	return ~crc;
}

meta_info meta_from_buffer(uint8_t *buff, size_t size) {
	meta_info meta;
	meta.length = htole32(size);
	meta.hash = htole32(crc32m(buff, size));
	return meta;
}

bool check_buffer_meta(meta_info *meta, uint8_t *buff) {
	return meta->hash == crc32m(buff, meta->length);
}

int send_buffer(int fd, void *buff, uint32_t msg_size) {
	meta_info meta = meta_from_buffer(buff, msg_size);

	for (size_t offset = 0; offset < sizeof(meta);) {
		offset += send(fd, &meta + offset, sizeof(meta) - offset, 0);
	}

	int sent = 0;
	for (size_t offset = 0; offset < msg_size; offset += sent) {
		sent += send(fd, buff + offset, msg_size - sent, 0);
	}
	return sent;
}

bool recv_message(struct pollfd *pfd, uint8_t *msg, size_t size) {
	int recved = 0;
	size_t offset = 0;

	while (true) {
		if (poll(pfd, 1, 100) > 0) {
			if (pfd->revents & POLLIN) {
				pfd->revents = 0;
				while ((recved = recv(pfd->fd, (void *) msg + offset, size - offset, MSG_DONTWAIT)) > 0) {
					offset += recved;
					if (offset == size) {
						return true;
					}
				}
			} else if (pfd->revents) {
				return false;
			}
		}
	}

}

void *listener(void *arg) {
    int id = *(int *)arg;

	struct pollfd pfd = { clients[id].fd, POLLIN, 0 };

	while (pfd.revents == 0) {
		meta_info meta;

		if (!recv_message(&pfd, (uint8_t *) &meta, sizeof(meta_info))) {
			continue;
		}

		meta.length = le32toh(meta.length);
		meta.hash = le32toh(meta.hash);

		uint8_t *buff = (uint8_t *) alloca(meta.length);

		if (!recv_message(&pfd, buff, meta.length)) {
			continue;
		}

		if (check_buffer_meta(&meta, buff)) {
			message_handler(id, buff, meta.length, /*proto_msg->data, proto_msg->n_data, proto_msg->type,*/ &pfd.revents);
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
