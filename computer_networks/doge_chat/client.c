#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <signal.h>

#include "chatgui.h"
#include "protocol.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <poll.h>

client clients[] = { 0 };

int socket_fd;
pthread_t listen_thread;

void message_handler(int id, char *msg, int *revents) {
	gtk_text_buffer_insert_at_cursor(buffer_chat, msg, strlen(msg));
}

void errproto_handler(int id, int *revents) {
	*revents |= POLLERR;
}

void dc_handler(int id) {
	gtk_label_set_text(label_state, "Offline");
}

int init_connect(char* ip_addr, int port_num) {
	struct sockaddr_in sin = { 0 };
	int socket_fd;

	sin.sin_addr.s_addr = inet_addr(ip_addr);
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

int main(int argc, char* argv[]) {
	init_gui(argc, argv);
}

#include "signals.h"
