#pragma once

#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#include "chatgui.h"

extern int socket_fd;
extern pthread_t listen_thread;
extern void* listener(void *arg);

void send_msg();
void disconnect();
void save_dialog_open();
void login_dialog_open();
void login_dialog_cancel();
void login_dialog_ok();
void connect_dialog_open();
void connect_dialog_cancel();
void connect_dialog_connect();
gboolean on_key_press(GtkWidget* widget, GdkEventKey* pKey, gpointer userdata);
