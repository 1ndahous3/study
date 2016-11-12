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

#define BUFFER_SIZE 32
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
	KICKED
};

char *recv_msg[] = {
		"* Welcome to the chatroom",
		"* Oops, too much clients\n",
		"* Login is incorrect, try another\n",
		"* Login is already in use\n",
		"* You have been kicked by admin\n"
};

struct client {
	bool isAlive;
	int fd;
	char nickName[LOGIN_LENGTH];
};

struct client clients[MAX_CLIENTS];

void kick(struct client client, char *msg);
void disconnect(struct client client);

void broadcast(int id, char *msg) {
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].isAlive && i != id) {
			send(clients[i].fd, clients[id].nickName, strlen(clients[id].nickName), 0);
			send(clients[i].fd, ": ", 2, 0);
			send(clients[i].fd, msg, strlen(msg), 0);
			send(clients[i].fd, "\n", 1, 0);
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

bool check_login(char *str) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].isAlive && (strncmp(str, clients[i].nickName, LOGIN_LENGTH) == 0)) {
			return false;
		}
	}
	return true;
}

void *client_session(void *arg) {
	char buff[BUFFER_SIZE];
    int id = *(int *)arg;
    int bytes_recv;

    //login
    char login[LOGIN_LENGTH] = { 0 };
    pthread_mutex_lock(&mutex);
    bytes_recv = recv(clients[id].fd, login, sizeof(login), MSG_NOSIGNAL);
    switch (bytes_recv)
    {
    	case -1:
    		perror("* Receiving massage");
    		clients[id].isAlive = false;
    		return NULL;
    	case 0:
    		printf("* Client was left from the server.\n");
    		clients[id].isAlive = false;
    		return NULL;
    }


    if (errno == EPIPE) {
    	printf("* Client communication error, disconnecting.\n");
    	clients[id].isAlive = false;
    	return NULL;
    }

    if (strlen(login) == 0) {
    	kick(clients[id], recv_msg[LOGIN_INCORRECT]);
    	return NULL;
    }

    if (!check_login(login)) {
    	kick(clients[id], recv_msg[LOGIN_EXIST]);
    	return NULL;
    }

	strncpy(clients[id].nickName, login, LOGIN_LENGTH);
	pthread_mutex_unlock(&mutex);
	printf("* Client was joined under name \"%s\"\n", clients[id].nickName);

	send(clients[id].fd, recv_msg[WELCOME], strlen(recv_msg[WELCOME]), MSG_NOSIGNAL);
	send(clients[id].fd, ", ", 2, MSG_NOSIGNAL);
	send(clients[id].fd, login, strlen(login), MSG_NOSIGNAL);
	send(clients[id].fd, "\n", 1, MSG_NOSIGNAL);

	while(true) {
		memset(buff, 0, sizeof(buff));
		bytes_recv = recv(clients[id].fd, buff, sizeof(buff), MSG_NOSIGNAL | MSG_PEEK);
		switch (bytes_recv) {
		case -1:
			perror("Receiving massage");
			clients[id].isAlive = false;
			return NULL;
		case 0:
			printf("* %s was left from the server.\n", clients[id].nickName);
			clients[id].isAlive = false;
			return NULL;
		default:
		    pthread_mutex_lock(&mutex);

		    int i = 0;

		    while((bytes_recv = recv(clients[id].fd, buff + i, sizeof(buff) - i, MSG_DONTWAIT)) > 0) {
		    	i += bytes_recv;
		    }
		    pthread_mutex_unlock(&mutex);

			if (strncmp("!list", buff, 5) == 0) {
				send_client_list(id);
				continue;
			}
			printf("%s: %s\n", clients[id].nickName, buff);
			broadcast(id, buff);
		}

	}
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
				if (pthread_create(&client_thread, NULL, client_session, &i)
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
		char buff[BUFFER_SIZE];
		memset(buff, 0, sizeof(buff));
		fgets(buff, BUFFER_SIZE, stdin);
		if (strncmp(buff, "!kick", 5) == 0) {
			char login[BUFFER_SIZE];
			strncpy(login, buff + 6, BUFFER_SIZE - 5);
			printf("%s\n", login);
			for (int i = 0; i < MAX_CLIENTS; i++) {
				if (clients[i].isAlive && (strncmp(login, clients[i].nickName, strnlen(clients[i].nickName, LOGIN_LENGTH)) == 0)) {
					kick(clients[i], "* Kicked by admin\n");
					break;
				}
			}
		} else if (strncmp(buff, "!exit", 5)) {
			exit(0);
		}
	}
}

void disconnect(struct client client) {
	printf("* Client has been disconnected from the server");
	if (strlen(client.nickName) != 0) {
		printf(": %s", client.nickName);
	}
	printf(".\n");
	client.isAlive = false;
	if (client.fd) {
		shutdown(client.fd, 1);
		close(client.fd);
	}
}

void kick(struct client client, char *msg) {
	if (client.isAlive) {
		send(client.fd, recv_msg[KICKED], strlen(recv_msg[KICKED]), MSG_NOSIGNAL);
		send(client.fd, msg, strlen(msg), MSG_NOSIGNAL);
		disconnect(client);
	}
}

