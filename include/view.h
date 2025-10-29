#ifndef VIEW_H
#define VIEW_H

#include <gtk/gtk.h>
#include <glib.h>
#include "model.h"

GtkWidget *view_create_main_window(GtkApplication *app);
GtkWidget *view_get_add_button(void);
GtkWidget *view_get_delete_button(void); // ÚJ DEKLARÁCIÓ
GtkWidget *view_get_entry_field(void);
GtkWidget *view_get_search_entry(void); // ÚJ
GtkWidget *view_get_list_box(void);
void view_refresh_list(GList *list);
gint view_get_selected_index(void);
#endif
