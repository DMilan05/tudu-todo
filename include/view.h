#ifndef VIEW_H
#define VIEW_H

#include <gtk/gtk.h>
#include "model.h"

/* --- F� ablak --- */
GtkWidget *view_create_main_window(GtkApplication *app);

/* --- Getterek --- */
GtkWidget *view_get_add_button(void);
GtkWidget *view_get_delete_button(void);
GtkWidget *view_get_save_button(void);
GtkWidget *view_get_mark_done_button(void);
GtkWidget *view_get_edit_button(void);
GtkWidget *view_get_search_entry(void);
GtkWidget *view_get_sort_dropdown(void);
GtkWidget *view_get_list_box(void);

/* --- Lista friss�t�se --- */
void view_refresh_list(GList *list);

/* --- Kieg�sz�tett deklar�ci�k --- */
GtkWidget *view_create_add_window(GtkWindow *parent);
GtkWidget *view_create_edit_window(GtkWindow *parent, TodoItem *item);

#endif /* VIEW_H */
