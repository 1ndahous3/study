#pragma once

#include <pthread.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#define _TITLE_ "Doge Chat v0.2"

GtkWidget		*window;
GtkBuilder		*builder;
GtkDialog		*dialog_connect,
				*dialog_login;
GtkTextBuffer	*buffer_chat,
				*buffer_send;
GtkEntryBuffer	*buffer_login,
				*buffer_ip,
				*buffer_port;
GtkLabel		*label_state,
				*label_waddr;

char 			main_title[50],
				*login,
				*ip_addr;
int				port_num;

void init_gui(int argc, char* argv[]);
