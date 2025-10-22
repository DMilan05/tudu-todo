#include "controller.h"
#include "model.h"
#include "view.h"

void controller_init(GtkWidget *window);
void controller_on_add_clicked(GtkButton *button, gpointer user_data);
void controller_on_delete_clicked(GtkButton *button, gpointer user_data);
void controller_on_save_clicked(GtkButton *button, gpointer user_data);
g_signal_connect(add_button, "clicked", G_CALLBACK(controller_on_add_clicked), NULL);

