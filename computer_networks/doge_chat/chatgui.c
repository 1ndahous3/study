#include "chatgui.h"

void init_gui(int argc, char* argv[]) {
	GtkTextView	*area_chat,
				*area_send;
	GtkEntry	*entry_login,
				*entry_ip,
				*entry_port;

	gtk_init(&argc, &argv);
	id 			= 0;
	builder		= gtk_builder_new();
	gtk_builder_add_from_file(builder, "client.glade", NULL);

	window		= GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
	area_chat	= GTK_TEXT_VIEW(gtk_builder_get_object(builder, "chat_area"));
	area_send	= GTK_TEXT_VIEW(gtk_builder_get_object(builder, "send_area"));
	entry_login	= GTK_ENTRY(gtk_builder_get_object(builder, "entry_login"));
	entry_ip	= GTK_ENTRY(gtk_builder_get_object(builder, "entry_ip"));
	entry_port	= GTK_ENTRY(gtk_builder_get_object(builder, "entry_port"));
	label_state	= GTK_LABEL(gtk_builder_get_object(builder, "label_state"));
	label_waddr	= GTK_LABEL(gtk_builder_get_object(builder, "label_wrong_address"));
	buffer_chat	= gtk_text_view_get_buffer(area_chat);
	buffer_send	= gtk_text_view_get_buffer(area_send);
	buffer_login= gtk_entry_get_buffer(entry_login);
	buffer_ip	= gtk_entry_get_buffer(entry_ip);
	buffer_port	= gtk_entry_get_buffer(entry_port);

	dialog_connect	= GTK_DIALOG(gtk_builder_get_object(builder, "dialog_connect"));
	dialog_login	= GTK_DIALOG(gtk_builder_get_object(builder, "dialog_login"));

	//gtk_dialog_add_button(dialog_connect, "test", GTK_RESPONSE_CANCEL);


	//send_button = GTK_WIDGET(gtk_builder_get_object(builder, "send_button"));

	gtk_builder_connect_signals(builder, NULL);
	login = "";
	gtk_window_set_title(GTK_WINDOW(window), _TITLE_);

	g_object_unref(builder);

	gtk_widget_show(window);

	/*
	if (pthread_create(&gui_thread, NULL, gui_main, NULL) != 0) {
		perror("Creating the listener thread...\n");
		return;
	}*/

	gtk_main();

}

void clean_buffer_chat() {
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(buffer_chat, &start, &end);
	gtk_text_buffer_delete(buffer_chat, &start, &end);
}
