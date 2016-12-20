#include "server.h"

int main(int argc, char* argv[]) {
	ip_addr = "127.0.0.1";
	port_num = 3490;

	pthread_mutex_init(&mutex, NULL);
	struct sockaddr_in sin = { 0 };
	int socket_fd;

	inet_pton(AF_INET, ip_addr, &sin.sin_addr);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port_num);

	pthread_t serv_thread;

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
					kick_user(&clients[i], "* Kicked by admin\n");
					break;
				}
			}
		} else if (strncmp(buff, "!exit", 5)) {
			exit(0);
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

void message_handler(int id, char **msg, uint32_t count, Type type, short int *revents) {
	switch (type)
	{
	case TYPE__text:
		if (!clients[id].isLogin) {
			if (strlen(msg[0]) == 0) {
				kick_user(&clients[id], recv_msg[LOGIN_INCORRECT]);
				*revents |= POLLHUP;
			} else if (!log_in(id, msg[0])) {
				kick_user(&clients[id], recv_msg[LOGIN_EXIST]);
				*revents |= POLLHUP;
			}
			return;
		}
		break;
	case TYPE__list:
		send_client_list(id);
		return;
	case TYPE__dc:
		kick_user(&clients[id], "* Disconnected");
		*revents |= POLLHUP;
		return;
	default: break;
	}
	for (uint32_t i = 0; i < count; i++) {
		printf("%s: %s\n", clients[id].nickName, msg[i]);
		broadcast(id, msg[i]);
	}
}

void errproto_handler(int id, short int *revents) {
	kick_user(&clients[id], recv_msg[ERR_PROTO]);
	*revents |= POLLERR;
}

void dc_handler(int id) {
	clients[id].isAlive = false;
	clients[id].isLogin = false;
}

void broadcast(int id, char *msg) {
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].isAlive && i != id) {
			char **smsg = (char **)alloca(4 * sizeof(char*));
			smsg[0] = clients[id].nickName;
			smsg[1] = ": ";
			smsg[2] = msg;
			smsg[3] = "\n";

			send_buffer(clients[i].fd, smsg, 4, TYPE__text);
		}
	}
	pthread_mutex_unlock(&mutex);
}

void send_client_list(int id) {
	pthread_mutex_lock(&mutex);
	char **smsg = (char **)alloca(active_users * sizeof(char*));

	for (int i = 0, j = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].isAlive) {
			smsg[j++] = clients[i].nickName;
		}
	}
	send_buffer(clients[id].fd, smsg, active_users, TYPE__list);
	pthread_mutex_unlock(&mutex);
}

bool log_in(int id, char *msg) {
	if (check_login(msg)) {
		strncpy(clients[id].nickName, msg, LOGIN_LENGTH);
		clients[id].isLogin = true;
		active_users++;
		printf("* Client was joined under name \"%s\"\n", clients[id].nickName);
		char **smsg = (char **) alloca(4 * sizeof(char*));
		smsg[0] = recv_msg[WELCOME];
		smsg[1] = ", ";
		smsg[2] = msg;
		smsg[3] = "\n";
		send_buffer(clients[id].fd, smsg, 4, TYPE__text);

		return true;
	}
	return false;
}

bool check_login(char *msg) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].isAlive && (strncmp(msg, clients[i].nickName, LOGIN_LENGTH) == 0)) {
			return false;
		}
	}
	return true;
}

void disconnect_user(client *client) {
	printf("* Client has been disconnected from the server");
	if (strlen(client->nickName) != 0) {
		printf(": %s", client->nickName);
	}
	printf(".\n");
	client->isAlive = false;
	client->isLogin = false;
	active_users--;
	if (client->fd) {
		shutdown(client->fd, 1);
		close(client->fd);
	}
}

void kick_user(client *client, char *msg) {
	if (client->isAlive) {
		char **smsg = (char **) alloca(2 * sizeof(char*));
		smsg[0] = recv_msg[KICKED];
		smsg[1] = msg;
		send_buffer(client->fd, smsg, 2, TYPE__text);
		send_buffer(client->fd, NULL, 0, TYPE__dc);
		disconnect_user(client);
	}
}

