#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include <pthread.h>

#define PORT "3490"

#define BUFFER_SIZE 32
#define MAX_CLIENTS 10
#define LOGIN_LENGTH 10

enum recv_map {
	SUCC,
	LIMIT,
	LOGIN,
	LOGIN_CORRECT
};

char *recv_msg[] = {
		"Welcome to the chatroom\n",
		"Oops, too much clients\n",
		"Enter your nickname, please:\n",
		"Login is correct\n"
};

struct client {
	bool isAlive;
	int fd;
	char nickName[LOGIN_LENGTH];
	//pthread_t thread;
};

struct client clients[MAX_CLIENTS];


void sigpipe_handler (int s) {

}

void broadcast(int id, char *msg) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
			if (clients[i].isAlive && i != id) {
				send(clients[i].fd, clients[id].nickName, strlen(clients[id].nickName), 0);
				send(clients[i].fd, "> ", 2, 0);
				send(clients[i].fd, msg, strlen(msg), 0);
				send(clients[i].fd, "\n", 1, 0);
			}
		}
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
	//signal(SIGPIPE, sigpipe_handler);

	char buff[BUFFER_SIZE];
    int id = *(int *)arg;
    int bytes_sent;


    send(clients[id].fd, recv_msg[SUCC], strlen(recv_msg[SUCC]), 0);

    //login
    char login[LOGIN_LENGTH];
	do {
		memset(login, 0, sizeof(login));
		send(clients[id].fd, recv_msg[LOGIN], strlen(recv_msg[LOGIN]), MSG_NOSIGNAL);
		recv(clients[id].fd, login, sizeof(login), 0);
		if (errno == EPIPE) {
			printf("* Client communication error, disconnecting.\n");
			clients[id].isAlive = false;
			return NULL;
		}
	} while (!check_login(login));

	strncpy(clients[id].nickName, login, LOGIN_LENGTH);
	printf("* Client was joined under name \"%s\"\n", clients[id].nickName);

	send(clients[id].fd, recv_msg[LOGIN_CORRECT], strlen(recv_msg[LOGIN_CORRECT]), 0);

	while(true) {

		bytes_sent = recv(clients[id].fd, buff, sizeof(buff), 0);
		switch (bytes_sent) {
		case -1:
			perror("Receiving massage.");
			clients[id].isAlive = false;
			return (void*)0;
		case 0:
			printf("* %s was left from the server.\n", clients[id].nickName);
			clients[id].isAlive = false;
			return (void*)0;
		default:
			printf("%s: %s\n", clients[id].nickName, buff);
			broadcast(id, buff);
		}

	}
}

void *server_session(void *arg) {
	/*socklen_t addr_size;
    struct sockaddr_storage their_addr;
    pthread_t client_thread;
    int client_fd, socket_fd = *(int *)arg;

	while (true) {
		addr_size = sizeof their_addr;
		client_fd = accept(socket_fd, (struct sockaddr *) &their_addr,
				&addr_size);
		printf("Client connected.\n");
		char buff[1024];



		for (int i = 0; i < MAX_CLIENTS; i++) {
			if (clients[i].isAlive == false) {

				clients[i].fd = client_fd;
				if (pthread_create(&client_thread, NULL, client_session, &i) != 0) {
					perror("Creating the client thread");
					return (void*)0;
				}

				clients[i].isAlive = true;
				break;
			}
			if (i == MAX_CLIENTS - 1) {
				printf("Connection refused.\n");
				send(client_fd, recv_msg[LIMIT], strlen(recv_msg[LIMIT]), 0);
			}
		}
	}*/
}

int main(int argc, char* argv[]) {
	//signal(SIGPIPE, sigpipe_handler);

	int ret;

	pthread_t serv_thread, client_thread;

    socklen_t addr_size;
    struct addrinfo hints, *res;
    int socket_fd;

	memset(&hints, 0, sizeof hints);

	hints.ai_family = AF_UNSPEC;  // использовать IPv4 или IPv6, нам неважно
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;     // Заполните для меня мой IP

	if ((ret = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
	    fprintf(stderr, "getaddrinfo error: %sn", gai_strerror(ret));
	    return 1;
	}

	if ((socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
		perror("Socket was not created;\n");
		return 1;
	}

	int n = 1;

	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(int));


	if (bind(socket_fd, res->ai_addr, res->ai_addrlen) == -1) {
		perror("Unable to bind socket;\n");
		return 1;
	}

	if (listen(socket_fd, 2) == -1) {
		perror("Listen error;\n");
		return 1;
	}

	printf("Listening for incoming connections...\n");
	int client_fd;

	struct sockaddr_storage their_addr;
	addr_size = sizeof their_addr;


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
					perror("Creating the client thread");
					return -1;
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
/*
		char buff[32];
		memset(buff, 0, sizeof(buff));
		int bytes_received;
		while ((bytes_received = recv(client_fd, buff, sizeof buff - 1, 0))) {
			printf("%s", buff);
			memset(buff, 0, sizeof(buff));
		}
		sleep(1);
*/
/*
	if (pthread_create(&serv_thread, NULL, server_session, &socket_fd) != 0) {
		perror("Creating the listener thread");
		return 1;
	}
	while (true) {}
/*

*/}

