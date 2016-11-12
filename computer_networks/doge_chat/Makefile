SERVER_NAME=server
CLIENT_NAME=client
SERVER_SRCS=server.c
CLIENT_SRCS=client.c chatgui.c signals.c
CFLAGS=-g -pthread -Wall -o
GTKFLAGS=-export-dynamic `pkg-config --cflags --libs gtk+-3.0`
CC=gcc

all: server client
	server $$ client

server: $(SERVER_SRCS)
	$(CC) $(CFLAGS) $(SERVER_NAME) $(SERVER_SRCS)

client: $(CLIENT_SRCS)
	$(CC) $(CFLAGS) $(CLIENT_NAME) $(CLIENT_SRCS) $(GTKFLAGS)

clean:
	/bin/rm -f $(CLIENT_NAME) $(SERVER_NAME)