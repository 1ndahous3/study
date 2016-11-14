#pragma once

#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#include "chatgui.h"
#include "protocol.h"
#include "client.h"


extern int socket_fd;
extern pthread_t listen_thread;

void send_msg();
void disconnect_user();
void save_dialog_open();
void login_dialog_open();
void login_dialog_cancel();
void login_dialog_ok();
void connect_dialog_open();
void connect_dialog_cancel();
void connect_dialog_connect();

gboolean login_entry_key_press(GtkWidget* widget, GdkEventKey* pKey, gpointer userdata);
gboolean connect_entry_key_press(GtkWidget* widget, GdkEventKey* pKey, gpointer userdata);
gboolean send_area_key_press(GtkWidget* widget, GdkEventKey* pKey, gpointer userdata);
