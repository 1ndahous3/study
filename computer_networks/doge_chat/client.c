#include "client.h"

int main(int argc, char* argv[]) {
	init_gui(argc, argv);
}

int init_connect(char* ip_addr, int port_num) {
	struct sockaddr_in sin = { 0 };
	int socket_fd;

	inet_pton(AF_INET, ip_addr, &sin.sin_addr);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port_num);

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Creating socket");
		return 0;
	}

	if (connect(socket_fd, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
		perror("Connection");
		return 0;
	}

	return socket_fd;
}

int send_message(int fd, char **msg, uint32_t count, Type type) {
	ClientMessage proto_msg = CLIENT_MESSAGE__INIT;
	proto_msg.type = type;
	switch(type) {
	case TYPE__text:
		proto_msg.data = msg;
		proto_msg.n_data = count;
		break;
	case TYPE__login:
		proto_msg.login = *msg;
		break;
	default: break;
	}
	uint32_t msg_size = client_message__get_packed_size(&proto_msg);
	void *buff = alloca(msg_size);
	client_message__pack(&proto_msg, buff);
	return send_buffer(fd, buff, msg_size);
}

void message_handler(int id, uint8_t *buff, uint32_t length, short int *revents) {
	ServerMessage *proto_msg = server_message__unpack(NULL, length, buff);
	switch (proto_msg->type) {
	case TYPE__list:
		for (uint32_t i = 0; i < proto_msg->n_userlist; i++) {
			gtk_text_buffer_insert_at_cursor(buffer_chat, "[", 1);
			gtk_text_buffer_insert_at_cursor(buffer_chat, proto_msg->userlist[i],
					strlen(proto_msg->userlist[i]));
			gtk_text_buffer_insert_at_cursor(buffer_chat, "]", 1);
		}
		gtk_text_buffer_insert_at_cursor(buffer_chat, "\n", 1);
		break;
	case TYPE__dc:
		dc_handler(id);
		break;
	case TYPE__text:
		for (uint32_t i = 0; i <  proto_msg->n_data; i++) {
			gtk_text_buffer_insert_at_cursor(buffer_chat, proto_msg->data[i],
					strlen(proto_msg->data[i]));
		}
		gtk_text_buffer_insert_at_cursor(buffer_chat, "\n", 1);
		break;
	default: break;
	}

	server_message__free_unpacked(proto_msg, NULL);
}

/*
void message_handler(int id, char **msg, uint32_t count, Type type, short int *revents) {
	switch (type) {
	case TYPE__text:
		for (uint32_t i = 0; i < count; i++) {
			gtk_text_buffer_insert_at_cursor(buffer_chat, msg[i],
					strlen(msg[i]));
		}
		gtk_text_buffer_insert_at_cursor(buffer_chat, "\n", 1);
		break;
	case TYPE__list:
		for (uint32_t i = 0; i < count; i++) {
			gtk_text_buffer_insert_at_cursor(buffer_chat, "[", 1);
			gtk_text_buffer_insert_at_cursor(buffer_chat, msg[i],
					strlen(msg[i]));
			gtk_text_buffer_insert_at_cursor(buffer_chat, "]", 1);
		}
		gtk_text_buffer_insert_at_cursor(buffer_chat, "\n", 1);
		break;
	case TYPE__dc:
		dc_handler(id);
		break;
	}
}*/

void errproto_handler(int id, short int *revents) {
	*revents |= POLLERR;
}

void dc_handler(int id) {
	gtk_label_set_text(label_state, "Offline");
}

#include "signals.h"
