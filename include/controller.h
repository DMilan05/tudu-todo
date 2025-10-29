#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <gtk/gtk.h>

void controller_init(GtkWidget *window);
void controller_on_add_clicked(GtkButton *button, gpointer user_data);
void controller_on_delete_clicked(GtkButton *button, gpointer user_data);
void controller_on_save_clicked(GtkButton *button, gpointer user_data);

#endif
