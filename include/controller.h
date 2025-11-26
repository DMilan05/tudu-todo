#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <gtk/gtk.h>

// Inicializálja a controllert és a főablak eseménykezelőit.
void controller_init(GtkWidget *window);

// A főablak gombjainak eseménykezelői.
void controller_on_add_clicked(GtkButton *button, gpointer user_data);
void controller_on_delete_clicked(GtkButton *button, gpointer user_data);
void controller_on_mark_done_clicked(GtkButton *button, gpointer user_data);
void controller_on_save_clicked(GtkButton *button, gpointer user_data);
void controller_on_edit_clicked(GtkButton *button, gpointer user_data);
void controller_on_sort_changed(GtkDropDown *dropdown, GParamSpec *pspec, gpointer user_data);
void controller_on_delete_completed_clicked(GtkButton *button, gpointer user_data);

// Az ablak bezárási kérelmét kezeli (pl. mentés felajánlása).
gboolean controller_on_close_request(GtkWindow *window, gpointer user_data);

// Az alkalmazás leállításakor fut le, felszabadítja a memóriát.
void controller_shutdown(void);

#endif
