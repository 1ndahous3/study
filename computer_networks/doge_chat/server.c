#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdlib.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include <pthread.h>

#include "protocol.h"

#include <poll.h>


#define MAX_CLIENTS 10
#define LOGIN_LENGTH 10

char *ip_addr = "127.0.0.1";
int	port_num = 3490;

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

void kick_user(client client, char *msg);
void disconnect_user(client client);

void broadcast(int id, char *msg) {
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].isAlive && i != id) {
			send_massage(clients[i].fd, clients[id].nickName);
			send_massage(clients[i].fd, ": ");
			send_massage(clients[i].fd, msg);
			send_massage(clients[i].fd, "\n");
		}
	}
	pthread_mutex_unlock(&mutex);
}

void send_client_list(int id) {
	pthread_mutex_lock(&mutex);
	send(clients[id].fd, "[", 1, 0);
	send(clients[id].fd, clients[id].nickName, strlen(clients[id].nickName), 0);

	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].isAlive && i != id) {
			send(clients[id].fd, ", ", 2, 0);
			send(clients[id].fd, clients[i].nickName, strlen(clients[i].nickName), 0);
		}

	}
	send(clients[id].fd, "]\n", 3, 0);
	pthread_mutex_unlock(&mutex);
}

bool check_login(char *msg) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].isAlive && (strncmp(msg, clients[i].nickName, LOGIN_LENGTH) == 0)) {
			return false;
		}
	}
	return true;
}

bool log_in(int id, char *msg) {
	if (check_login(msg)) {
		strncpy(clients[id].nickName, msg, LOGIN_LENGTH);
		clients[id].isLogin = true;
		printf("* Client was joined under name \"%s\"\n", clients[id].nickName);
		send_massage(clients[id].fd, recv_msg[WELCOME]);
		send_massage(clients[id].fd, ", ");
		send_massage(clients[id].fd, msg);
		send_massage(clients[id].fd, "\n");
		return true;
	}
	return false;
}

void message_handler(int id, char *msg, int *revents) {
	if (!clients[id].isLogin) {
		if (strlen(msg) == 0) {
			kick_user(clients[id], recv_msg[LOGIN_INCORRECT]);
			*revents |= POLLHUP;
		} else if (!log_in(id, msg)) {
			kick_user(clients[id], recv_msg[LOGIN_EXIST]);
			*revents |= POLLHUP;
		}
		return;
	}
	printf("%s: %s\n", clients[id].nickName, msg);
	broadcast(id, msg);
}

void errproto_handler(int id, int *revents) {
	kick_user(clients[id], recv_msg[ERR_PROTO]);
	*revents |= POLLERR;
}

void dc_handler(int id) {
	clients[id].isAlive = false;
	clients[id].isLogin = false;
}

void *server_session(void *arg) {
	socklen_t addr_size;
    struct sockaddr_storage their_addr;
    pthread_t client_thread;
	int client_fd, socket_fd = *(int *) arg;

	while (true) {
		addr_size = sizeof their_addr;
		client_fd = accept(socket_fd, (struct sockaddr *) &their_addr,
				&addr_size);
		printf("* Client connected.\n");

		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (clients[i].isAlive == false) {
				clients[i].fd = client_fd;
				if (pthread_create(&client_thread, NULL, listener, &i)
						!= 0) {
					perror("* Creating the client thread");
					return NULL;
				}

				clients[i].isAlive = true;
				break;
			}
			if (i == MAX_CLIENTS - 1) {
				printf("* Connection refused.\n");
				send(client_fd, recv_msg[LIMIT], strlen(recv_msg[LIMIT]), 0);
			}
		}
	}
}

int main(int argc, char* argv[]) {

	pthread_mutex_init(&mutex, NULL);
	struct sockaddr_in sin = { 0 };
	int socket_fd;

	sin.sin_addr.s_addr = inet_addr(ip_addr);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port_num);

	int ret;

	pthread_t serv_thread, client_thread;

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Creating socket");
		return 1;
	}

	int n = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(int));

	if (bind(socket_fd, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
		perror("Binding socket");
		return 1;
	}

	if (listen(socket_fd, 2) == -1) {
		perror("Listening");
		return 1;
	}

	printf("Listening for incoming connections...\n");

	if (pthread_create(&serv_thread, NULL, server_session, &socket_fd) != 0) {
		perror("* Creating the listener thread");
		return -1;
	}

	while (true) {
		char buff[MAX_MESSAGE_LENGTH];
		memset(buff, 0, sizeof(buff));
		fgets(buff, MAX_MESSAGE_LENGTH, stdin);
		if (strncmp(buff, "!kick", 5) == 0) {
			char login[MAX_MESSAGE_LENGTH];
			strncpy(login, buff + 6, MAX_MESSAGE_LENGTH - 5);
			printf("%s\n", login);
			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (clients[i].isAlive && (strncmp(login, clients[i].nickName, strnlen(clients[i].nickName, LOGIN_LENGTH)) == 0)) {
					kick_user(clients[i], "* Kicked by admin\n");
					break;
				}
			}
		} else if (strncmp(buff, "!exit", 5)) {
			exit(0);
		}
	}
}


void disconnect_user(client client) {
	printf("* Client has been disconnected from the server");
	if (strlen(client.nickName) != 0) {
		printf(": %s", client.nickName);
	}
	printf(".\n");
	client.isAlive = false;
	client.isLogin = false;
	if (client.fd) {
		shutdown(client.fd, 1);
		close(client.fd);
	}
}

void kick_user(client client, char *msg) {
	if (client.isAlive) {
		send(client.fd, recv_msg[KICKED], strlen(recv_msg[KICKED]), MSG_NOSIGNAL);
		send(client.fd, msg, strlen(msg), MSG_NOSIGNAL);
		disconnect_user(client);
	}
}

