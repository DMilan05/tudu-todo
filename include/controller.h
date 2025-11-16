#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <gtk/gtk.h>

/* Inicializálás – fõablakhoz */
void controller_init(GtkWidget *window);

/* (Ezeket már nem kell mind exportálni, de nem árt) */
void controller_on_add_clicked(GtkButton *button, gpointer user_data);
void controller_on_delete_clicked(GtkButton *button, gpointer user_data);
void controller_on_mark_done_clicked(GtkButton *button, gpointer user_data);
void controller_on_save_clicked(GtkButton *button, gpointer user_data);
void controller_on_edit_clicked(GtkButton *button, gpointer user_data);
void controller_on_sort_changed(GtkDropDown *dropdown, GParamSpec *pspec, gpointer user_data);

/* Leállítás – mentéshez és memória felszabadításhoz */
void controller_shutdown(void);

#endif
