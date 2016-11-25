#pragma once

#include <pthread.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#define _TITLE_ "Doge Chat v0.4"

GtkWidget		*window;
GtkBuilder		*builder;
GtkDialog		*dialog_connect,
				*dialog_login;
GtkAboutDialog	*dialog_about;
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
int				id,
				port_num;

void init_gui(int argc, char* argv[]);
void clean_buffer_chat();
