#ifndef SEARCH_H
#define SEARCH_H

#include <gtk/gtk.h>
#include "model.h"

void search_init(GtkSearchEntry *search_entry, GtkListBox *list_box, GList **todos_ptr);
GList *search_filter(GList *list, const gchar *query);

#endif
