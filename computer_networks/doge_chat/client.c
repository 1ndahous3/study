#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <netinet/in.h>

#include <string.h>
#include <stdio.h>

#include <pthread.h>


#define BUFFER_SIZE 32

#define LOGIN_LENGTH 10

void* listener(void *arg) {
	int fd = *(int *)arg;
	char buff[BUFFER_SIZE];

	while(1) {
		memset(buff, 0, sizeof(buff));
		recv(fd, buff, sizeof buff, 0);
		printf("%s", buff);
	}
}

int main(int argc, char* argv[]) {

	pthread_t client_thread;

	int ret;

	struct sockaddr_storage their_addr;
	socklen_t addr_size;
	struct addrinfo hints, *res;
	int sockfd, new_fd;

	memset(&hints, 0, sizeof hints);

	hints.ai_family = AF_UNSPEC;  // использовать IPv4 или IPv6, нам неважно
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;     // Заполните для меня мой IP

	if ((ret = getaddrinfo(NULL, "3490", &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo error: %sn", gai_strerror(ret));
		return 1;
	}

	if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol))
			== -1) {
		perror("Socket was not created;\n");
		return 1;
	}

	if ((ret = connect(sockfd, res->ai_addr, res->ai_addrlen)) == -1) {
		perror("Unable to connect;\n");
		return 1;
	}


	if (pthread_create(&client_thread, NULL, listener, &sockfd) != 0) {
		perror("Creating the listener thread");
		return 1;
	}

	char buff[BUFFER_SIZE];
	while (1) {
		memset(buff, 0, sizeof(buff));
		fgets(buff, BUFFER_SIZE, stdin);
		buff[strcspn(buff, "\n")] = '\0';
		send(sockfd, buff, strlen(buff) + 1, 0);
	}
}

