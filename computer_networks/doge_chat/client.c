#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <signal.h>

#include "chatgui.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <poll.h>

int socket_fd;
pthread_t listen_thread;

//limits
#define BUFFER_SIZE 32
#define LOGIN_LENGTH 32

void* listener(void *arg) {
	char buffer[BUFFER_SIZE];
	struct pollfd pfd = { *(int *)arg, 0, 0 };

	while (pfd.revents == 0) {
		poll(&pfd, 1, 100);
		memset(buffer, 0, sizeof(buffer));

		while (recv(pfd.fd, buffer, sizeof(buffer), MSG_DONTWAIT) > 0) {
			printf("%s\n", (char *)buffer_chat);
			gtk_text_buffer_insert_at_cursor(buffer_chat, buffer,
									strlen(buffer));
			memset(buffer, 0, sizeof(buffer));
		}
	}
	if (pfd.revents & POLLERR) {
		printf("* An error has occurred on the stream.\n");
	}
	if (pfd.revents & POLLHUP) {
		printf("* Connection has been closed.\n");
	}
	if (pfd.revents & POLLNVAL) {
		printf("* The specified fd value is invalid.\n");
	}

	gtk_label_set_text(label_state, "Offline");

	return NULL;
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

void clean_buffer_chat() {
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(buffer_chat, &start, &end);
	gtk_text_buffer_delete(buffer_chat, &start, &end);
}

#include "signals.h"
